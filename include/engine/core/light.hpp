#pragma once
#include <glm/glm.hpp>
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

        glm::mat4 GetProjectionMatrix() const;
        glm::vec3 GetDirection(const Transform& transform) const;
        glm::vec3 GetColor() const;

    };
}