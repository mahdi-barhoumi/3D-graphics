#pragma once
#include <engine/core/math.hpp>
#include <engine/core/color.hpp>
#include <engine/core/transform.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Light : public Component
    {
        private:

        Color m_Color = Color::White;
        float m_Length = 100.0f;
        float m_NearPlane = 0.0f;
        float m_FarPlane = 100.0f;

        public:

        Light() = default;
        Light(const Light& other) = default;
        ~Light() = default;

        Matrix4 GetProjectionMatrix() const;
        Vector3 GetLocalDirection() const;
        Vector3 GetWorldDirection(const Transform& transform) const;
        Vector3 GetColor() const;

    };
}