#include <stdexcept>
#include <glew/glew.hpp>
#include <engine/core/window.hpp>

namespace Engine
{
    GLFWwindow* Window::Initialize()
    {
        if (glfwInit() != GLFW_TRUE) throw std::runtime_error("Could not initialize context manager.");
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4); 
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        GLFWwindow* mainWindow = glfwCreateWindow(1, 1, "Engine", nullptr, nullptr);
        glfwDefaultWindowHints();
        if (!mainWindow) throw std::runtime_error("Could not create main context.");
        glfwMakeContextCurrent(mainWindow);
        if (glewInit() != GLEW_OK) throw std::runtime_error("Could not initialize main context.");
        return mainWindow;
    }
    GLFWwindow* Window::s_MainWindow = Initialize();
    GLFWwindow* Window::s_CurrentWindow = nullptr;
    Window::Window() { Create(); }
    Window::Window(std::string title) : m_Title(title) { Create(); }
    Window::Window(std::string title, unsigned int width, unsigned int height) : m_Title(title), m_Width(width), m_Height(height), m_AspectRatio(static_cast<float>(width) / static_cast<float>(height)) { Create(); }
    Window::~Window()
    {
        glfwDestroyWindow(mp_Window);
        for (auto input : m_Inputs) { input->mp_Window = nullptr; }
    }
    void Window::Create()
    {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4); 
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        mp_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        glfwDefaultWindowHints();
        if (!mp_Window) throw std::runtime_error("Could not create window.");
        // glfwMakeContextCurrent(mp_Window);
        // glfwSwapInterval(0);
        // glfwMakeContextCurrent(s_MainWindow);
        HGLRC context = glfwGetWGLContext(mp_Window);
        wglDeleteContext(context);
        glfwSetWindowUserPointer(mp_Window, this);
        glfwSetCursorPos(mp_Window, m_Width / 2, m_Height / 2);
        glfwSetInputMode(mp_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(mp_Window, WindowCursorPositionCallback);
        glfwSetWindowSizeCallback(mp_Window, WindowSizeCallback);
    }
    void Window::RegisterInput(Input* input)
    {
        input->mp_Window = this;
        m_Inputs.push_back(input);
    }
    void Window::UnregisterInput(Input* input)
    {
        for (size_t i = 0; i < m_Inputs.size(); ++i)
        {
            if (input == m_Inputs.at(i))
            {
                std::swap(m_Inputs.at(i), m_Inputs.back());
                m_Inputs.pop_back();
                input->mp_Window = nullptr;
                return;
            }
        }
    }
    void Window::PushKeyToInputs(const Key& key)
    {
        for (auto input : m_Inputs)
        {
            if (input->m_KeyQueue.size() >= Input::s_MaxQueueSize) input->m_KeyQueue.pop();
            input->m_KeyQueue.push(key);
        }
    }
    void Window::PushMovementToInputs(const Movement& movement)
    {
        for (auto input : m_Inputs)
        {
            if (input->m_MovementQueue.size() >= Input::s_MaxQueueSize) input->m_MovementQueue.pop();
            input->m_MovementQueue.push(movement);
        }
    }
    bool Window::ShouldClose() { return glfwWindowShouldClose(mp_Window); }
    void Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        if (width == 0 || height == 0) return;
        Window* p_Window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        p_Window->m_Width = width;
        p_Window->m_Height = height;
        p_Window->m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }
    void Window::WindowCursorPositionCallback(GLFWwindow* window, double x, double y)
    {
        Window* p_Window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        float deltaX = (x - static_cast<double>(p_Window->m_Width) * 0.5 ) / static_cast<double>(p_Window->m_Width);
        float deltaY = - (y - static_cast<double>(p_Window->m_Height) * 0.5) / static_cast<double>(p_Window->m_Height);
        p_Window->PushMovementToInputs({deltaX, deltaY});
        glfwSetCursorPos(window, p_Window->m_Width / 2, p_Window->m_Height / 2);
    }
    void Window::MakeCurrent()
    {
        if (mp_Window != s_CurrentWindow)
        {
            wglMakeCurrent(GetDC(glfwGetWin32Window(mp_Window)), glfwGetWGLContext(s_MainWindow));
            s_CurrentWindow = mp_Window;
        }
    }
    void Window::SwapBuffers() { glfwSwapBuffers(mp_Window); }
    void Window::ProcessEvents()
    {
        glfwPollEvents();
        if (glfwGetMouseButton(mp_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) PushKeyToInputs(Key::MouseLeft);
        if (glfwGetMouseButton(mp_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) PushKeyToInputs(Key::MouseRight);
        if (glfwGetKey(mp_Window, GLFW_KEY_W) == GLFW_PRESS) PushKeyToInputs(Key::Z);
        if (glfwGetKey(mp_Window, GLFW_KEY_A) == GLFW_PRESS) PushKeyToInputs(Key::Q);
        if (glfwGetKey(mp_Window, GLFW_KEY_S) == GLFW_PRESS) PushKeyToInputs(Key::S);
        if (glfwGetKey(mp_Window, GLFW_KEY_D) == GLFW_PRESS) PushKeyToInputs(Key::D);
        if (glfwGetKey(mp_Window, GLFW_KEY_Q) == GLFW_PRESS) PushKeyToInputs(Key::A);
        if (glfwGetKey(mp_Window, GLFW_KEY_E) == GLFW_PRESS) PushKeyToInputs(Key::E);
        if (glfwGetKey(mp_Window, GLFW_KEY_SPACE) == GLFW_PRESS) PushKeyToInputs(Key::Space);
        if (glfwGetKey(mp_Window, GLFW_KEY_UP) == GLFW_PRESS) PushKeyToInputs(Key::UpArrow);
        if (glfwGetKey(mp_Window, GLFW_KEY_DOWN) == GLFW_PRESS) PushKeyToInputs(Key::DownArrow);
        if (glfwGetKey(mp_Window, GLFW_KEY_LEFT) == GLFW_PRESS) PushKeyToInputs(Key::LeftArrow);
        if (glfwGetKey(mp_Window, GLFW_KEY_RIGHT) == GLFW_PRESS) PushKeyToInputs(Key::RightArrow);
        if (glfwGetKey(mp_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) PushKeyToInputs(Key::LeftShift);
        if (glfwGetKey(mp_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) PushKeyToInputs(Key::LeftControl);
    }
    void Window::SetTitle(std::string title)
    {
        glfwSetWindowTitle(mp_Window, title.c_str());
        m_Title = title;
    }
    std::string Window::GetTitle() { return m_Title; }
    unsigned int Window::GetWidth() { return m_Width; }
    unsigned int Window::GetHeight() { return m_Height; }
    unsigned int Window::GetInputsCount() { return m_Inputs.size(); }
    float Window::GetAspectRatio() { return m_AspectRatio; }
}