#pragma once
#include <queue>
#include <memory>
#include <engine/core/component.hpp>

namespace Engine
{
    enum class Key
    {
        None,
        Right,
        Left,
        Down,
        Up
    };

    class Window;

    class Input : public Component
    {
        private:

        std::weak_ptr<std::vector<std::weak_ptr<std::queue<Key>>>> m_InputQueuesWeak;
        std::shared_ptr<std::queue<Key>> m_Queue = std::make_shared<std::queue<Key>>();

        void Unregister();

        public:

        Input() = default;
        Input(Window& window);
        Input(const Input& other);
        Input(Input&& other) noexcept;
        ~Input();
        Input& operator=(const Input& other);
        Input& operator=(Input&& other) noexcept;

        void Clear();
        Key GetLastKey();
        Key GetFirstKey();
        Key PopFirstKey();

    };
}