#pragma once
#include <engine/core/component.hpp>

namespace Engine
{
    class World;
    class Window;

    enum class Key
    {
        UpArrow,
        DownArrow,
        LeftArrow,
        RightArrow,
        Space,
        LeftAlt,
        LeftShift,
        LeftControl,
        RightAlt,
        RightShift,
        RightControl,
        A, Z, E, R, T, Y, U, I, O, P, Q, S, D, F, G, H, J, K, L, M, W, X, C, V, B, N,
        Escape // Keep this escape key the last element because C++ doesn't have reflections.
    };
    enum class KeyState
    {
        Released,
        Pressed,
        Held
    };
    
    enum class MouseButton
    {
        MouseLeft,
        MouseRight,
        MouseMiddle // Same thing, keep it last.
    };
    enum class MouseButtonState
    {
        Released,
        Pressed,
        Held
    };
    
    struct CursorPosition
    {
        float x = 0.0f;
        float y = 0.0f;
    };
    struct CursorMovement
    {
        float dx = 0.0f;
        float dy = 0.0f;
    };

    class Controllable : public Component {};

    class Input
    {
        public:

        void Control(World& world, Window& window, float deltaTime) const;

    };
}