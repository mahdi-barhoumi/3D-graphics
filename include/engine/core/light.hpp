#pragma once
#include <glm/glm.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Light : public Component
    {
        private:

        glm::vec3 m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);

        public:

        Light() = default;
        Light(const Light& other) = default;
        ~Light() = default;

        glm::vec3 GetDirection() const;
        glm::mat4 GetProjectionMatrix() const;

    };
}
