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
        GLFWwindow* mainWindow = glfwCreateWindow(1, 1, "Engine", nullptr, nullptr);
        glfwDefaultWindowHints();
        if (!mainWindow) throw std::runtime_error("Could not create main context.");
        glfwMakeContextCurrent(mainWindow);
        if (glewInit() != GLEW_OK) throw std::runtime_error("Could not initialize main context.");
        return mainWindow;
    }
    GLFWwindow* Window::s_MainWindow = Initialize();
    Window::Window() { Create(); }
    Window::Window(std::string title) : m_Title(title) { Create(); }
    Window::Window(std::string title, unsigned int width, unsigned int height) : m_Title(title), m_Width(width), m_Height(height), m_AspectRatio(static_cast<float>(width) / static_cast<float>(height)) { Create(); }
    Window::~Window() { glfwDestroyWindow(mp_Window); }
    void Window::Create()
    {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4); 
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        mp_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        glfwDefaultWindowHints();
        if (!mp_Window) throw std::runtime_error("Could not create window.");
        HGLRC context = glfwGetWGLContext(mp_Window);
        wglDeleteContext(context);
        glfwSetWindowUserPointer(mp_Window, this);
        glfwSetWindowSizeCallback(mp_Window, WindowSizeCallback);
        m_InputQueues = std::make_shared<std::vector<std::weak_ptr<std::queue<Key>>>>();
    }
    void Window::PushKeyToInputQueues(Key key)
    {
        for (size_t i = 0; i < m_InputQueues->size();)
        {
            if (auto queue = m_InputQueues->at(i).lock())
            {
                queue->push(key);
                ++i;
            }
            else
            {
                std::swap(m_InputQueues->at(i), m_InputQueues->back());
                m_InputQueues->pop_back();
            }
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
    void Window::MakeCurrent() { wglMakeCurrent(GetDC(glfwGetWin32Window(mp_Window)), glfwGetWGLContext(s_MainWindow)); }
    void Window::SwapBuffers() { glfwSwapBuffers(mp_Window); }
    void Window::ProcessEvents()
    {
        glfwPollEvents();
        if (glfwGetKey(mp_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(mp_Window, GLFW_TRUE);
        if (glfwGetKey(mp_Window, GLFW_KEY_W) == GLFW_PRESS) PushKeyToInputQueues(Key::Up);
        if (glfwGetKey(mp_Window, GLFW_KEY_A) == GLFW_PRESS) PushKeyToInputQueues(Key::Left);
        if (glfwGetKey(mp_Window, GLFW_KEY_S) == GLFW_PRESS) PushKeyToInputQueues(Key::Down);
        if (glfwGetKey(mp_Window, GLFW_KEY_D) == GLFW_PRESS) PushKeyToInputQueues(Key::Right);
    }
    void Window::SetTitle(std::string title)
    {
        glfwSetWindowTitle(mp_Window, title.c_str());
        m_Title = title;
    }
    std::string Window::GetTitle() { return m_Title; }
    unsigned int Window::GetWidth() { return m_Width; }
    unsigned int Window::GetHeight() { return m_Height; }
    float Window::GetAspectRatio() { return m_AspectRatio; }

    int Window::GetNumQueues() { return m_InputQueues->size(); }
}