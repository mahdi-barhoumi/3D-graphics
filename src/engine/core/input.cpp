#include <algorithm>
#include <engine/core/input.hpp>
#include <engine/core/window.hpp>

namespace Engine
{
    Input::Input(Window& window) { window.RegisterInput(this); }
    Input::Input(const Input& other)
    {
        m_KeyQueue = other.m_KeyQueue;
        m_MovementQueue = other.m_MovementQueue;
        if (other.mp_Window) other.mp_Window->RegisterInput(this);
    }
    Input::Input(Input&& other) noexcept
    {
        m_KeyQueue = std::move(other.m_KeyQueue);
        m_MovementQueue = std::move(other.m_MovementQueue);
        if (other.mp_Window)
        {
            other.mp_Window->UnregisterInput(&other);
            other.mp_Window->RegisterInput(this);
        }
    }
    Input::~Input() { if (mp_Window) mp_Window->UnregisterInput(this); }
    Input& Input::operator=(const Input& other)
    {
        if (this == &other) return *this;
        m_KeyQueue = other.m_KeyQueue;
        m_MovementQueue = other.m_MovementQueue;
        if (mp_Window == other.mp_Window) return *this;
        if (mp_Window) mp_Window->UnregisterInput(this);
        if (other.mp_Window) other.mp_Window->RegisterInput(this);
        return *this;
    }
    Input& Input::operator=(Input&& other) noexcept
    {
        if (this == &other) return *this;
        if (mp_Window) mp_Window->UnregisterInput(this);
        m_KeyQueue = std::move(other.m_KeyQueue);
        m_MovementQueue = std::move(other.m_MovementQueue);
        if (other.mp_Window)
        {
            other.mp_Window->UnregisterInput(&other);
            other.mp_Window->RegisterInput(this);
        }
        return *this;
    }
    void Input::PullFrom(Window& window)
    {
        if (mp_Window == &window) return;
        if (mp_Window)
        {
            mp_Window->UnregisterInput(this);
            mp_Window = nullptr;
            window.RegisterInput(this);
            mp_Window = &window;
        }
        else
        {
            window.RegisterInput(this);
            mp_Window = &window;
        }
    }
    bool Input::HasKeys() { return !m_KeyQueue.empty(); }
    void Input::ClearKeys() { while(!m_KeyQueue.empty()) m_KeyQueue.pop(); }
    Key Input::GetLastKey()
    {
        if (m_KeyQueue.empty()) return Key::None;
        return m_KeyQueue.back();
    }
    Key Input::GetFirstKey()
    {
        if (m_KeyQueue.empty()) return Key::None;
        return m_KeyQueue.front();
    }
    Key Input::PopFirstKey()
    {
        if (m_KeyQueue.empty()) return Key::None;
        Key key = m_KeyQueue.front();
        m_KeyQueue.pop();
        return key;
    }
    bool Input::HasMovements() { return !m_MovementQueue.empty(); }
    void Input::ClearMovements() { while(!m_MovementQueue.empty()) m_MovementQueue.pop(); }
    Movement Input::GetLastMovement()
    {
        if (m_MovementQueue.empty()) return {0, 0};
        return m_MovementQueue.back();
    }
    Movement Input::GetFirstMovement()
    {
        if (m_MovementQueue.empty()) return {0, 0};
        return m_MovementQueue.front();
    }
    Movement Input::PopFirstMovement()
    {
        if (m_MovementQueue.empty()) return {0, 0};
        Movement movement = m_MovementQueue.front();
        m_MovementQueue.pop();
        return movement;
    }
}