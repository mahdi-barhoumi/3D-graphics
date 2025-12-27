#pragma once
#include <glm/glm.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    struct Light : public Component
    {
        glm::vec4 color;
        Light() = default;
        Light(const Light&) = default;
        ~Light() = default;
    };
}
