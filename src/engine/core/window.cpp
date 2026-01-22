#include <windows.h>
#include <glew/glew.hpp>
#include <engine/core/window.hpp>
#include <engine/core/framebuffer.hpp>

namespace Engine
{
    GLFWwindow* Window::s_MainWindow = Initialize();
    GLFWwindow* Window::s_CurrentWindow = nullptr;
    GLFWwindow* Window::Initialize()
    {
        if (glfwInit() != GLFW_TRUE) throw std::runtime_error("Could not initialize context manager.");
        glfwWindowHint(GLFW_SAMPLES, 4); 
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
        GLFWwindow* mainWindow = glfwCreateWindow(1, 1, "Engine", nullptr, nullptr);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        if (!mainWindow) throw std::runtime_error("Could not create main context.");
        glfwMakeContextCurrent(mainWindow);
        if (glewInit() != GLEW_OK) throw std::runtime_error("Could not initialize main context.");
        return mainWindow;
    }
    void Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        if (width == 0 || height == 0) return;
        Window* p_Window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        p_Window->m_Width = width;
        p_Window->m_Height = height;
        p_Window->m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }
    void Window::WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* p_Window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        Key changedKey;
        KeyState state;
        switch (key)
        {
            case GLFW_KEY_UP: changedKey = Key::UpArrow; break;
            case GLFW_KEY_DOWN: changedKey = Key::DownArrow; break;
            case GLFW_KEY_RIGHT: changedKey = Key::RightArrow; break;
            case GLFW_KEY_LEFT: changedKey = Key::LeftArrow; break;
            case GLFW_KEY_SPACE: changedKey = Key::Space; break;
            case GLFW_KEY_LEFT_ALT: changedKey = Key::LeftAlt; break;
            case GLFW_KEY_LEFT_SHIFT: changedKey = Key::LeftShift; break;
            case GLFW_KEY_LEFT_CONTROL: changedKey = Key::LeftControl; break;
            case GLFW_KEY_RIGHT_ALT: changedKey = Key::RightAlt; break;
            case GLFW_KEY_RIGHT_SHIFT: changedKey = Key::RightShift; break;
            case GLFW_KEY_RIGHT_CONTROL: changedKey = Key::RightControl; break;
            case GLFW_KEY_ESCAPE: changedKey = Key::Escape; break;
            case GLFW_KEY_Q: changedKey = Key::A; break;
            case GLFW_KEY_W: changedKey = Key::Z; break;
            case GLFW_KEY_E: changedKey = Key::E; break;
            case GLFW_KEY_R: changedKey = Key::R; break;
            case GLFW_KEY_T: changedKey = Key::T; break;
            case GLFW_KEY_Y: changedKey = Key::Y; break;
            case GLFW_KEY_U: changedKey = Key::U; break;
            case GLFW_KEY_I: changedKey = Key::I; break;
            case GLFW_KEY_O: changedKey = Key::O; break;
            case GLFW_KEY_P: changedKey = Key::P; break;
            case GLFW_KEY_A: changedKey = Key::Q; break;
            case GLFW_KEY_S: changedKey = Key::S; break;
            case GLFW_KEY_D: changedKey = Key::D; break;
            case GLFW_KEY_F: changedKey = Key::F; break;
            case GLFW_KEY_G: changedKey = Key::G; break;
            case GLFW_KEY_H: changedKey = Key::H; break;
            case GLFW_KEY_J: changedKey = Key::J; break;
            case GLFW_KEY_K: changedKey = Key::K; break;
            case GLFW_KEY_L: changedKey = Key::L; break;
            case GLFW_KEY_SEMICOLON: changedKey = Key::M; break;
            case GLFW_KEY_Z: changedKey = Key::W; break;
            case GLFW_KEY_X: changedKey = Key::X; break;
            case GLFW_KEY_C: changedKey = Key::C; break;
            case GLFW_KEY_V: changedKey = Key::V; break;
            case GLFW_KEY_B: changedKey = Key::B; break;
            case GLFW_KEY_N: changedKey = Key::N; break;
            default: return;
        }
        switch (action)
        {
            case GLFW_PRESS: state = KeyState::Pressed; break;
            case GLFW_RELEASE: state = KeyState::Released; break;
            case GLFW_REPEAT: state = KeyState::Held; break;
            default: return;
        }
        p_Window->m_KeyStates[static_cast<size_t>(changedKey)] = state;
    }
    void Window::WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Window* p_Window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        MouseButton changedButton;
        MouseButtonState state;
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_RIGHT: changedButton = MouseButton::MouseRight; break;
            case GLFW_MOUSE_BUTTON_LEFT: changedButton = MouseButton::MouseLeft; break;
            case GLFW_MOUSE_BUTTON_MIDDLE: changedButton = MouseButton::MouseMiddle; break;
            default: return;
        }
        switch (action)
        {
            case GLFW_PRESS: state = MouseButtonState::Pressed; break;
            case GLFW_RELEASE: state = MouseButtonState::Released; break;
            case GLFW_REPEAT: state = MouseButtonState::Held; break;
            default: return;
        }
        p_Window->m_MouseButtonStates[static_cast<size_t>(changedButton)] = state;
    }
    void Window::WindowCursorPositionCallback(GLFWwindow* window, double x, double y)
    {
        Window* p_Window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        
        CursorPosition lastCursorPosition = p_Window->m_CursorPosition;
        p_Window->m_CursorPosition = {
            .x = 2.0f * static_cast<float>(x) / static_cast<float>(p_Window->m_Width) - 1.0f,
            .y = 2.0f * static_cast<float>(-y) / static_cast<float>(p_Window->m_Height) + 1.0f,
        };
        p_Window->m_CursorMovementAccumulator.dx += p_Window->m_CursorPosition.x - lastCursorPosition.x;
        p_Window->m_CursorMovementAccumulator.dy += p_Window->m_CursorPosition.y - lastCursorPosition.y;

        if (p_Window->m_LockCursor)
        {
            glfwSetCursorPos(window, p_Window->m_Width / 2, p_Window->m_Height / 2);
            p_Window->m_CursorPosition = {0.0f, 0.0f};
        }
    }

    Window::Window() { Create(); }
    Window::Window(std::string title) : m_Title(title) { Create(); }
    Window::Window(unsigned int width, unsigned int height) : m_Width(width), m_Height(height), m_AspectRatio(static_cast<float>(width) / static_cast<float>(height)) { Create(); }
    Window::Window(std::string title, unsigned int width, unsigned int height) : m_Title(title), m_Width(width), m_Height(height), m_AspectRatio(static_cast<float>(width) / static_cast<float>(height)) { Create(); }
    Window::~Window() { glfwDestroyWindow(mp_Window); }
    
    void Window::Create()
    {
        mp_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        if (!mp_Window) throw std::runtime_error("Could not create window.");

        glfwMakeContextCurrent(mp_Window);
        glfwSwapInterval(0);
        glfwMakeContextCurrent(s_MainWindow);

        HWND hwnd = glfwGetWin32Window(mp_Window);
        HICON hAppIcon = (HICON) LoadImage(GetModuleHandle(NULL), "MAINICON", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

        if (hAppIcon)
        {
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) hAppIcon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hAppIcon);
        }

        wglDeleteContext(glfwGetWGLContext(mp_Window));
        glfwSetWindowUserPointer(mp_Window, this);
        glfwSetCursorPos(mp_Window, m_Width / 2, m_Height / 2);
        glfwSetInputMode(mp_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(mp_Window, WindowKeyCallback);
        glfwSetMouseButtonCallback(mp_Window, WindowMouseButtonCallback);
        glfwSetCursorPosCallback(mp_Window, WindowCursorPositionCallback);
        glfwSetWindowSizeCallback(mp_Window, WindowSizeCallback);
    }

    void Window::SetTitle(std::string title)
    {
        glfwSetWindowTitle(mp_Window, title.c_str());
        m_Title = title;
    }
    std::string Window::GetTitle() const { return m_Title; }
    float Window::GetAspectRatio() const { return m_AspectRatio; }
    unsigned int Window::GetWidth() const { return m_Width; }
    unsigned int Window::GetHeight() const { return m_Height; }
    bool Window::ShouldClose() const { return glfwWindowShouldClose(mp_Window); }
    void Window::MakeCurrent() const
    {
        Framebuffer::Default.SetWidth(m_Width);
        Framebuffer::Default.SetHeight(m_Height);
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
        m_LastCursorMovement = m_CursorMovementAccumulator;
        m_CursorMovementAccumulator = {0.0f, 0.0f};
    }
    bool Window::IsCursorLocked() { return m_LockCursor; }
    void Window::UnlockCursor()
    {
        m_LockCursor = false;
        glfwSetInputMode(mp_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    void Window::LockCursor()
    {
        m_LockCursor = true;
        m_CursorPosition = {0.0f, 0.0f};
        glfwSetCursorPos(mp_Window, m_Width / 2, m_Height / 2);
        glfwSetInputMode(mp_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool Window::IsKeyHeld(Key key) const { return m_KeyStates[static_cast<size_t>(key)] == KeyState::Held; }
    bool Window::IsKeyPressed(Key key) const { return m_KeyStates[static_cast<size_t>(key)] == KeyState::Pressed; }
    bool Window::IsKeyReleased(Key key) const { return m_KeyStates[static_cast<size_t>(key)] == KeyState::Released; }
    bool Window::IsMouseButtonHeld(MouseButton button) const { return m_MouseButtonStates[static_cast<size_t>(button)] == MouseButtonState::Held; }
    bool Window::IsMouseButtonPressed(MouseButton button) const { return m_MouseButtonStates[static_cast<size_t>(button)] == MouseButtonState::Pressed; }
    bool Window::IsMouseButtonReleased(MouseButton button) const { return m_MouseButtonStates[static_cast<size_t>(button)] == MouseButtonState::Released; }
    KeyState Window::GetKeyState(Key key) const { return m_KeyStates[static_cast<size_t>(key)]; }
    MouseButtonState Window::GetMouseButtonState(MouseButton button) const { return m_MouseButtonStates[static_cast<size_t>(button)]; }
    CursorPosition Window::GetCursorPosition() const { return m_CursorPosition; }
    CursorMovement Window::GetCursorMovement() const { return m_LastCursorMovement; }
}