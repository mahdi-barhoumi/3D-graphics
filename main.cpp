#include <glew/glew.hpp>
#include <glfw/glfw.hpp>
#include <glm/glm.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace std;
using namespace glm;

// Classes

class object
{
    friend class world;
    friend class renderer;
    private:
        GLuint vertexArrayID, vertexBufferID, indexBufferID, textureID;
        vector<GLfloat> vertexBuffer;
        vector<GLuint> indexBuffer;
        vector<vec3> vertexPositions;
        vector<vec2> vertexTextureCoordinates;
        vector<vec3> vertexNormals;
        vector<uvec3> vertexIndices;
        vec3 scale;
        vec3 position;
        vec3 rotation;
        mat4 scalingMatrix;
        mat4 rotationMatrix;
        mat4 translationMatrix;
        mat4 modelMatrix;
    public:
        object(filesystem::path modelFilePath)
        :   vertexArrayID(0), vertexBufferID(0), indexBufferID(0),
            scale(vec3(1, 1, 1)), position(vec3(0, 0, 0)), rotation(vec3(0, 0, 0)),
            scalingMatrix(mat4(1)), rotationMatrix(mat4(1)), translationMatrix(mat4(1)), modelMatrix(mat4(1))
        {            
            ifstream modelFile(modelFilePath, ios::in);
            if (modelFile && modelFilePath.extension().string() == ".obj") {
                string line;
                while (getline(modelFile, line)) {
                    float x, y, z;
                    unsigned int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
                    if (sscanf(line.c_str(), "v %f %f %f", &x, &y, &z) == 3) {
                        vertexPositions.push_back(vec3(x, y, z));
                    }
                    else if (sscanf(line.c_str(), "vt %f %f", &x, &y) == 2) {
                        vertexTextureCoordinates.push_back(vec2(x, y));
                    }
                    else if (sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z) == 3) {
                        vertexNormals.push_back(vec3(x, y, z));
                    }
                    else if (sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9) {
                        vertexIndices.push_back(vec3(v1, vt1, vn1));
                        vertexIndices.push_back(vec3(v2, vt2, vn2));
                        vertexIndices.push_back(vec3(v3, vt3, vn3));
                    }
                    else if (sscanf(line.c_str(), "f %u//%u %u//%u %u//%u", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6) {
                        vertexIndices.push_back(vec3(v1, 0, vn1));
                        vertexIndices.push_back(vec3(v2, 0, vn2));
                        vertexIndices.push_back(vec3(v3, 0, vn3));
                    }
                    else if (sscanf(line.c_str(), "f %u %u %u", &v1, &v2, &v3) == 3) {
                        vertexIndices.push_back(vec3(v1, 0, 0));
                        vertexIndices.push_back(vec3(v2, 0, 0));
                        vertexIndices.push_back(vec3(v3, 0, 0));
                    }
                }
                for (unsigned int i = 0; i < vertexIndices.size(); i++) {
                    indexBuffer.push_back(i);
                    vertexBuffer.push_back(vertexPositions[vertexIndices[i].x - 1].x);
                    vertexBuffer.push_back(vertexPositions[vertexIndices[i].x - 1].y);
                    vertexBuffer.push_back(vertexPositions[vertexIndices[i].x - 1].z);
                    if (vertexIndices[i].y > 0) {
                        vertexBuffer.push_back(vertexTextureCoordinates[vertexIndices[i].y - 1].s);
                        vertexBuffer.push_back(vertexTextureCoordinates[vertexIndices[i].y - 1].t);
                    }
                    else {
                        vertexBuffer.push_back(0);
                        vertexBuffer.push_back(0);
                    }
                    if (vertexIndices[i].z > 0) {
                        vertexBuffer.push_back(vertexNormals[vertexIndices[i].z - 1].x);
                        vertexBuffer.push_back(vertexNormals[vertexIndices[i].z - 1].y);
                        vertexBuffer.push_back(vertexNormals[vertexIndices[i].z - 1].z);
                    }
                    else {
                        vertexBuffer.push_back(0);
                        vertexBuffer.push_back(0);
                        vertexBuffer.push_back(0);
                    }
                }
                modelFile.close();
            }
            else {
                cout << "Error: Could not open model file " << modelFilePath.filename().string() << endl;
                modelFile.close();
                exit(-1);
            }
        }
        object(const object& object)
        :   vertexArrayID(0), vertexBufferID(0), indexBufferID(0),
            vertexBuffer(object.vertexBuffer), indexBuffer(object.indexBuffer),
            vertexPositions(object.vertexPositions), vertexTextureCoordinates(object.vertexTextureCoordinates), vertexNormals(object.vertexNormals), vertexIndices(object.vertexIndices),
            scale(object.scale), position(object.position), rotation(object.rotation),
            scalingMatrix(object.scalingMatrix), rotationMatrix(object.rotationMatrix), translationMatrix(object.translationMatrix), modelMatrix(object.modelMatrix)
        {

        }
        ~object()
        {
            unload();
        }
        const object& operator=(const object& object)
        {
            unload();
            vertexArrayID = 0;
            vertexBufferID = 0;
            indexBufferID = 0;
            vertexBuffer = object.vertexBuffer;
            indexBuffer = object.indexBuffer;
            vertexPositions = object.vertexPositions;
            vertexTextureCoordinates = object.vertexTextureCoordinates;
            vertexNormals = object.vertexNormals;
            vertexIndices = object.vertexIndices;
            scale = object.scale;
            position = object.position;
            rotation = object.rotation;
            scalingMatrix = object.scalingMatrix;
            rotationMatrix = object.rotationMatrix;
            translationMatrix = object.translationMatrix;
            modelMatrix = object.modelMatrix;
            return *this;
        }
        void load(void)
        {
            if (!vertexArrayID) {
                glGenVertexArrays(1, &vertexArrayID);
                glGenBuffers(1, &vertexBufferID);
                glGenBuffers(1, &indexBufferID);
                if (!vertexArrayID || !vertexBufferID || !indexBufferID) {
                    cout << "Error: Could not generate buffers" << endl;
                    exit(-1);
                }
                glBindVertexArray(vertexArrayID);
                glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
                glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(GLfloat), &vertexBuffer[0], GL_DYNAMIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (0 * sizeof(GLfloat)));
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (5 * sizeof(GLfloat)));
                glEnableVertexAttribArray(2);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(GLuint), &indexBuffer[0], GL_DYNAMIC_DRAW);
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                int width, height, numberChannels;
                unsigned char* data = stbi_load("./textures/dirt.bmp", &width, &height, &numberChannels, 0);
                if (data) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
                else {
                    cout << "Error: Could not open texture file." << endl;
                    exit(-1);
                }
                stbi_image_free(data);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
        }
        void unload(void)
        {
            if (vertexArrayID) {
                glDeleteVertexArrays(1, &vertexArrayID);
                glDeleteBuffers(1, &vertexBufferID);
                glDeleteBuffers(1, &indexBufferID);
            }
        }
        void moveTo(float x, float y, float z)
        {
            position = vec3(x, y, z);
        }
        void moveBy(float dx, float dy, float dz)
        {
            position += vec3(dx, dy, dz);
        }
        void rotate(float degreesX, float degreesY, float degreesZ)
        {
            rotation += vec3(radians(degreesX), radians(degreesY), radians(degreesZ));
        }
        void scaleTo(float factorX, float factorY, float factorZ)
        {
            if (factorX > 0) scale.x = factorX;
            if (factorY > 0) scale.y = factorY;
            if (factorZ > 0) scale.z = factorZ;
        }
        void update(void)
        {
            scalingMatrix = mat4(scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1);
            mat4 rotationXMatrix = mat4(1, 0, 0, 0, 0, cos(rotation.x), sin(rotation.x), 0, 0, -sin(rotation.x), cos(rotation.x), 0, 0, 0, 0, 1);
            mat4 rotationYMatrix = mat4(cos(rotation.y), 0, -sin(rotation.y), 0, 0, 1, 0, 0, sin(rotation.y), 0, cos(rotation.y), 0, 0, 0, 0, 1);
            mat4 rotationZMatrix = mat4(cos(rotation.z), sin(rotation.z), 0, 0, -sin(rotation.z), cos(rotation.z), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;
            translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, position.x, position.y, position.z, 1);
            modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
        }
};

class light
{
    friend class world;
    friend class renderer;
    private:
        vec4 color;
        vec3 position;
        vec3 direction;
    public:
        light(vec4 color = vec4(1, 1, 1, 1), vec3 position = vec3(0, 0, 50), vec3 direction = vec3(0, 0, -1))
        : color(color), position(position), direction(direction)
        {

        }
};

class camera
{
    friend class world;
    friend class renderer;
    private:
        float aspectRatio;
        float fieldOfView;
        float nearZ;
        float farZ;
        vec3 position;
        vec3 forward;
        vec3 right;
        vec3 up;
        mat4 translationMatrix;
        mat4 rotationMatrix;
        mat4 viewMatrix;
        mat4 projectionMatrix;
    public:
        camera(float aspectRatio = 1, float fieldOfView = 90, float nearZ = 0.1, float farZ = 100, float x = 0, float y = 0, float z = 0)
        : aspectRatio(aspectRatio), fieldOfView(radians(fieldOfView)), nearZ(nearZ), farZ(farZ), position(vec3(x, y, z))
        {
            forward = vec3(1, 0, 0);
            right = vec3(0, -1, 0);
            up = vec3(0, 0, 1);
            translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);
            rotationMatrix = mat4(right.x, up.x, forward.x, 0, right.y, up.y, forward.y, 0, right.z, up.z, forward.z, 0, 0, 0, 0, 1);
            viewMatrix = rotationMatrix * translationMatrix;
            projectionMatrix = mat4(1 / aspectRatio / tan(radians(fieldOfView) / 2), 0, 0, 0, 0, 1 / tan(radians(fieldOfView) / 2), 0, 0, 0, 0, (nearZ + farZ) / (farZ - nearZ), 1, 0, 0, 2 * nearZ * farZ / (nearZ - farZ), 0);
        }
        void setFieldOfView(float fieldOfView, bool degrees = true)
        {
            if (degrees) this->fieldOfView = radians(fieldOfView);
            else this->fieldOfView = fieldOfView;
        }
        void setNearClipPlane(float nearZ)
        {
            this->nearZ = nearZ;
        }
        void setFarClipPlane(float farZ)
        {
            this->farZ = farZ;
        }
        void moveTo(float x, float y, float z)
        {
            position = vec3(x, y, z);
        }
        void moveBy(float deltaX, float deltaY, float deltaZ)
        {
            position += vec3(deltaX, deltaY, deltaZ);
        }
        void moveForward(float value)
        {
            position += value * vec3(forward.x, forward.y, 0);
        }
        void moveBackward(float value)
        {
            position += value * vec3(-forward.x, -forward.y, 0);
        }
        void moveRight(float value)
        {
            position += value * vec3(right.x, right.y, 0);
        }
        void moveLeft(float value)
        {
            position += value * vec3(-right.x, -right.y, 0);
        }
        void moveUp(float value)
        {
            position += value * vec3(0, 0, up.z);
        }
        void moveDown(float value)
        {
            position += value * vec3(0, 0, -up.z);
        }
        void pan(float degrees)
        {
            quat pan = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * vec3(0, 0, 1));
            forward = pan * forward * conjugate(pan);
            right = pan * right * conjugate(pan);
            up = pan * up * conjugate(pan);
        }
        void roll(float degrees)
        {
            quat roll = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * forward);
            right = roll * right * conjugate(roll);
            up = roll * up * conjugate(roll);
        }
        void pitch(float degrees)
        {
            quat pitch = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * right);
            up = pitch * up * conjugate(pitch);
            forward = pitch * forward * conjugate(pitch);
        }
        void yaw(float degrees)
        {
            quat yaw = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * up);
            forward = yaw * forward * conjugate(yaw);
            right = yaw * right * conjugate(yaw);
        }
        void update(void)
        {
            translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);
            rotationMatrix = mat4(right.x, up.x, forward.x, 0, right.y, up.y, forward.y, 0, right.z, up.z, forward.z, 0, 0, 0, 0, 1);
            viewMatrix = rotationMatrix * translationMatrix;
            projectionMatrix = mat4(1 / aspectRatio / tan(fieldOfView / 2), 0, 0, 0, 0, 1 / tan(fieldOfView / 2), 0, 0, 0, 0, (nearZ + farZ) / (farZ - nearZ), 1, 0, 0, 2 * nearZ * farZ / (nearZ - farZ), 0);
        }
};

class world
 {
    friend class renderer;
    private:
        camera camera;
        vector<object> objects;
        vector<light> lights;
    public:
        world()
        {

        }
        void addObject(const object& object)
        {
            objects.push_back(object);
        }
        void addLight(const light& light)
        {
            lights.push_back(light);
        }
        void setCamera(const class camera& camera)
        {
            this->camera = camera;
        }
        void load(void)
        {
            for (unsigned int i = 0; i < objects.size(); i++) objects[i].load();
        }
        void unload(void)
        {
            for (unsigned int i = 0; i < objects.size(); i++) objects[i].unload();
        }
        void update(void)
        {

        }
 };

class renderer
{
    private:
        GLFWwindow* window;
        int windowWidth, windowHeight;
        GLuint program;
        GLint vertexPositionTransformationMatrixUniform;
        GLint vertexNormalTransformationMatrixUniform;
        GLint lightTransformationMatrixUniform;
        world scene;
    public:
        renderer(const world& scene = world())
        : scene(scene), windowWidth(1280), windowHeight(720)
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
        static void windowSizeCallback(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
            renderer* rendererPointer = (renderer*) glfwGetWindowUserPointer(window);
            rendererPointer->windowWidth = width;
            rendererPointer->windowHeight = height;
            rendererPointer->scene.camera.aspectRatio = (float) width / (float) height;
        }
        static void cursorPositionCallback(GLFWwindow* window, double x, double y)
        {
            renderer* rendererPointer = (renderer*) glfwGetWindowUserPointer(window);
            rendererPointer->scene.camera.pan(90 - 180 * x / (float) rendererPointer->windowWidth);
            rendererPointer->scene.camera.pitch(90 - 180 * y / (float) rendererPointer->windowHeight);
            glfwSetCursorPos(window, rendererPointer->windowWidth / 2, rendererPointer->windowHeight / 2);
        }
        void render()
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
        void mainloop(void)
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
};

int main(void)
{
    world world;
    object cube("./models/ayoub.obj");
    cube.scaleTo(0.1, 0.1, 0.1);
    world.addObject(cube);
    world.update();
    camera camera;
    camera.moveTo(-5, 0, 0);
    world.setCamera(camera);
    renderer system(world);
    system.mainloop();
    return 0;
}