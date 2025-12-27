#include <algorithm>
#include <engine/core/input.hpp>
#include <engine/core/window.hpp>

namespace Engine
{
    Input::Input(Window& window)
    {
        window.m_InputQueues->push_back(m_Queue);
        m_InputQueuesWeak = window.m_InputQueues;
    }
    Input::Input(const Input& other)
    {
        m_Queue = std::make_shared<std::queue<Key>>(*other.m_Queue);
        if (auto pointer = other.m_InputQueuesWeak.lock())
        {
            m_InputQueuesWeak = other.m_InputQueuesWeak;
            pointer->push_back(m_Queue);
        }
    }
    Input::Input(Input&& other) noexcept : m_Queue(std::move(other.m_Queue)), m_InputQueuesWeak(std::move(other.m_InputQueuesWeak)) {}
    Input::~Input() { Unregister(); }
    Input& Input::operator=(const Input& other)
    {
        if (this == &other) return *this;
        Unregister();
        m_Queue = std::make_shared<std::queue<Key>>(*other.m_Queue);
        if (auto pointer = other.m_InputQueuesWeak.lock())
        {
            m_InputQueuesWeak = other.m_InputQueuesWeak;
            pointer->push_back(m_Queue);
        }
        else m_InputQueuesWeak.reset();
        return *this;
    }
    Input& Input::operator=(Input&& other) noexcept
    {
        if (this == &other) return *this;
        m_Queue = std::move(other.m_Queue);
        m_InputQueuesWeak = std::move(other.m_InputQueuesWeak);
        return *this;
    }
    void Input::Unregister()
    {
        if (auto queues = m_InputQueuesWeak.lock())
        {
            for (size_t i = 0; i < queues->size();)
            {
                if (auto queue = queues->at(i).lock())
                {
                    if (queue == m_Queue)
                    {
                        std::swap(queues->at(i), queues->back());
                        queues->pop_back();
                        continue;
                    }
                    ++i;
                }
                else
                {
                    std::swap(queues->at(i), queues->back());
                    queues->pop_back();
                }
            }
        }
    }
    void Input::Clear() { while(!m_Queue->empty()) m_Queue->pop(); }
    Key Input::GetLastKey()
    {
        if (m_Queue->empty()) return Key::None;
        return m_Queue->back();
    }
    Key Input::GetFirstKey()
    {
        if (m_Queue->empty()) return Key::None;
        return m_Queue->front();
    }
    Key Input::PopFirstKey()
    {
        if (m_Queue->empty()) return Key::None;
        Key key = m_Queue->front();
        m_Queue->pop();
        return key;
    }
}