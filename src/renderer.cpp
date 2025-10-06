#include <renderer.hpp>
#include <glew/glew.hpp>
#include <glfw/glfw.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

renderer::renderer(const world& scene_)
: scene(scene_), windowWidth(1280), windowHeight(720)
{
    if (glfwInit() != GLFW_TRUE) exit(-1);
    if (!(window = glfwCreateWindow(windowWidth, windowHeight, "3D", NULL, NULL))) {
        glfwTerminate();
        exit(-1);
    }
    this->scene.camera.aspectRatio = (float) windowWidth / (float) windowHeight;
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(-1);
    }
    ifstream vertexShaderSourceFile("./shaders/vertexShader.glsl", ios::in);
    ifstream fragmentShaderSourceFile("./shaders/fragmentShader.glsl", ios::in);
    if (!vertexShaderSourceFile || !fragmentShaderSourceFile) {
        vertexShaderSourceFile.close();
        fragmentShaderSourceFile.close();
        exit(-1);
    }
    string vertexShaderSource, fragmentShaderSource;
    stringstream buffer;
    buffer << vertexShaderSourceFile.rdbuf();
    vertexShaderSource = buffer.str();
    buffer.str("");
    buffer << fragmentShaderSourceFile.rdbuf();
    fragmentShaderSource = buffer.str();
    vertexShaderSourceFile.close();
    fragmentShaderSourceFile.close();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* source;
    source = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &source, nullptr);
    source = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &source, nullptr);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
        GLchar log[length];
        glGetShaderInfoLog(vertexShader, length, &length, log);
        cout << "Failed to compile vertex shader:" << endl;
        cout << log << endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        exit(-1);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
        GLchar log[length];
        glGetShaderInfoLog(fragmentShader, length, &length, log);
        cout << "Failed to compile fragment shader:" << endl;
        cout << log << endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        exit(-1);
    }
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(program);
    vertexPositionTransformationMatrixUniform = glGetUniformLocation(program, "vertexPositionTransformationMatrix");
    vertexNormalTransformationMatrixUniform = glGetUniformLocation(program, "vertexNormalTransformationMatrix");
    lightTransformationMatrixUniform = glGetUniformLocation(program, "lightTransformationMatrix");
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void renderer::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    renderer* rendererPointer = (renderer*) glfwGetWindowUserPointer(window);
    rendererPointer->windowWidth = width;
    rendererPointer->windowHeight = height;
    rendererPointer->scene.camera.aspectRatio = (float) width / (float) height;
}

void renderer::cursorPositionCallback(GLFWwindow* window, double x, double y)
{
    renderer* rendererPointer = (renderer*) glfwGetWindowUserPointer(window);
    rendererPointer->scene.camera.pan(90 - 180 * x / (float) rendererPointer->windowWidth);
    rendererPointer->scene.camera.pitch(90 - 180 * y / (float) rendererPointer->windowHeight);
    glfwSetCursorPos(window, rendererPointer->windowWidth / 2, rendererPointer->windowHeight / 2);
}

void renderer::render()
{
    scene.camera.update();
    for (unsigned int i = 0; i < scene.objects.size(); i++) {
        scene.objects[i].update();
        mat4 vertexPositionTransformationMatrix = scene.camera.projectionMatrix * scene.camera.viewMatrix * scene.objects[i].modelMatrix;
        mat4 vertexNormalTransformationMatrix = scene.camera.rotationMatrix * scene.objects[i].rotationMatrix;
        mat4 lightTransformationMatrix = scene.camera.rotationMatrix;
        glBindVertexArray(scene.objects[i].vertexArrayID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.objects[i].indexBufferID);
        glUniformMatrix4fv(vertexPositionTransformationMatrixUniform, 1, GL_FALSE, &vertexPositionTransformationMatrix[0][0]);
        glUniformMatrix4fv(vertexNormalTransformationMatrixUniform, 1, GL_FALSE, &vertexNormalTransformationMatrix[0][0]);
        glUniformMatrix4fv(lightTransformationMatrixUniform, 1, GL_FALSE, &lightTransformationMatrix[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scene.objects[i].textureID);
        glUniform1i(glGetUniformLocation(program, "sampler"), 0);
        glDrawElements(GL_TRIANGLES, scene.objects[i].indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
    }
}

void renderer::mainloop(void)
{
    scene.load();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) scene.camera.moveForward(0.05);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) scene.camera.moveBackward(0.05);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) scene.camera.moveRight(0.05);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) scene.camera.moveLeft(0.05);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) scene.camera.moveUp(0.05);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) scene.camera.moveDown(0.05);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) scene.camera.roll(1);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) scene.camera.roll(-1);
        render();
        scene.update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    scene.unload();
    glfwDestroyWindow(window);
}
