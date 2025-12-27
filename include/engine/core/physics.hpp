#pragma once
#include <glm/glm.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    struct PhysicsComponent : public Component
    {
        float mass = 1.0f;
        float friction = 0.0f;
        glm::vec3 velocity = glm::vec3(0);
        glm::vec3 acceleration = glm::vec3(0);
        PhysicsComponent() = default;
        PhysicsComponent(const PhysicsComponent&) = default;
        ~PhysicsComponent() = default;
    };
}
