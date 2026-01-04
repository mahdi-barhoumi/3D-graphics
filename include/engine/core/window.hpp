#pragma once
#include <string>
#include <vector>
#include <glfw/glfw.hpp>
#include <glfw/glfwnative.hpp>
#include <engine/core/input.hpp>

namespace Engine
{
    class Window
    {
        friend class Input;

        private:

        std::string m_Title;
        unsigned int m_Width = 640;
        unsigned int m_Height = 480;
        float m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
        GLFWwindow* mp_Window = nullptr;
        std::vector<Input*> m_Inputs;

        void Create();
        void RegisterInput(Input* input);
        void UnregisterInput(Input* input);
        void PushKeyToInputs(const Key& key);
        void PushMovementToInputs(const Movement& movement);

        static GLFWwindow* s_MainWindow;
        static GLFWwindow* s_CurrentWindow;
        static GLFWwindow* Initialize();
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void WindowCursorPositionCallback(GLFWwindow* window, double x, double y);

        public:

        Window();
        Window(std::string title);
        Window(std::string title, unsigned int width, unsigned int height);
        ~Window();

        std::string GetTitle();
        void SetTitle(std::string title);
        float GetAspectRatio();
        unsigned int GetWidth();
        unsigned int GetHeight();
        unsigned int GetInputsCount();
        bool ShouldClose();
        void MakeCurrent();
        void SwapBuffers();
        void ProcessEvents();

    };
}
