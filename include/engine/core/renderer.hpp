#pragma once
#include <engine/core/world.hpp>
#include <engine/core/shader.hpp>

namespace Engine
{
    class Window;

    class Renderer
    {
        private:

        Shader m_Shader;
        Shader m_GridShader;

        GLuint gridVBO, gridVAO;

        void Initialize();

        public:

        Renderer();
        ~Renderer() = default;

        void Render(World& world, Window& window);

    };
}


// #pragma once

// #include <glew/glew.hpp>
// #include <glfw/glfw.hpp>

// #include <world.hpp>

// class renderer
// {
//     private:
//         GLFWwindow* window;
//         int windowWidth, windowHeight;
//         GLuint program;
//         GLint vertexPositionTransformationMatrixUniform;
//         GLint vertexNormalTransformationMatrixUniform;
//         GLint lightTransformationMatrixUniform;
//         world& scene;
//     public:
//         renderer(world& scene);
//         static void windowSizeCallback(GLFWwindow* window, int width, int height);
//         static void cursorPositionCallback(GLFWwindow* window, double x, double y);
//         void render();
//         void mainloop(void);
// };
