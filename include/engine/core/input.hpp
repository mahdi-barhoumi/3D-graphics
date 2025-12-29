#pragma once
#include <queue>
#include <memory>
#include <engine/core/component.hpp>

namespace Engine
{
    enum class Key
    {
        None,
        MouseRight,
        MouseLeft,
        Space,
        UpArrow,
        DownArrow,
        LeftArrow,
        RightArrow,
        LeftShift,
        LeftControl,
        A, D, E, S, Q, Z
    };

    struct Movement
    {
        float deltaX = 0;
        float deltaY = 0;
    };

    class Window;

    class Input : public Component
    {
        friend class Window;

        private:

        Window* mp_Window = nullptr;
        std::queue<Key> m_KeyQueue;
        std::queue<Movement> m_MovementQueue;

        static constexpr unsigned int s_MaxQueueSize = 255;

        public:

        Input() = default;
        Input(Window& window);
        Input(const Input& other);
        Input(Input&& other) noexcept;
        ~Input();
        Input& operator=(const Input& other);
        Input& operator=(Input&& other) noexcept;

        void PullFrom(Window& window);

        bool HasKeys();
        void ClearKeys();
        Key GetLastKey();
        Key GetFirstKey();
        Key PopFirstKey();
        bool HasMovements();
        void ClearMovements();
        Movement GetLastMovement();
        Movement GetFirstMovement();
        Movement PopFirstMovement();

    };
}
