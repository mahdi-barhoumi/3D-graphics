#pragma once
#include <array>
#include <queue>
#include <vector>
#include <string>
#include <glfw/glfw.hpp>
#include <glfw/glfwnative.hpp>
#include <engine/core/input.hpp>

namespace Engine
{
    class Window
    {
        private:

        GLFWwindow* mp_Window = nullptr;

        std::string m_Title;
        unsigned int m_Width = 640;
        unsigned int m_Height = 480;
        float m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

        bool m_LockCursor = true;
        CursorPosition m_CursorPosition;
        CursorMovement m_LastCursorMovement;
        CursorMovement m_CursorMovementAccumulator;
        std::array<KeyState, static_cast<size_t>(Key::Escape) + 1> m_KeyStates = {KeyState::Released};
        std::array<MouseButtonState, static_cast<size_t>(MouseButton::MouseMiddle) + 1> m_MouseButtonStates = {MouseButtonState::Released};

        void Create();

        static GLFWwindow* s_CurrentWindow;
        static GLFWwindow* s_MainWindow;
        static GLFWwindow* Initialize();
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void WindowCursorPositionCallback(GLFWwindow* window, double x, double y);

        public:

        Window();
        Window(std::string title);
        Window(unsigned int width, unsigned int height);
        Window(std::string title, unsigned int width, unsigned int height);
        ~Window();

        std::string GetTitle() const;
        void SetTitle(std::string title);
        float GetAspectRatio() const;
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        bool ShouldClose() const;
        void MakeCurrent() const;
        void SwapBuffers();
        void ProcessEvents();
        bool IsCursorLocked();
        void UnlockCursor();
        void LockCursor();

        bool IsKeyHeld(Key key) const;
        bool IsKeyPressed(Key key) const;
        bool IsKeyReleased(Key key) const;
        bool IsMouseButtonHeld(MouseButton button) const;
        bool IsMouseButtonPressed(MouseButton button) const;
        bool IsMouseButtonReleased(MouseButton button) const;
        KeyState GetKeyState(Key key) const;
        MouseButtonState GetMouseButtonState(MouseButton button) const;
        CursorPosition GetCursorPosition() const;
        CursorMovement GetCursorMovement() const;

    };
}
