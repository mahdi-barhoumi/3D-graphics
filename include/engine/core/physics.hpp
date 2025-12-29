#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <engine/core/collider.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Physics : public Component
    {
        friend class Solver;

        private:

        float m_Mass = 1.0f;
        float m_Friction = 0.0f;
        glm::vec3 m_Velocity = glm::vec3(0);
        glm::vec3 m_Acceleration = glm::vec3(0);
        std::shared_ptr<Collider> m_Collider = nullptr;

        public:

        Physics() = default;
        template<ColliderConcept T>
        Physics(T&& collider) : m_Collider(std::make_shared<T>(std::forward<T>(collider))) {}
        template<ColliderConcept T>
        Physics(T&& collider, float mass) : m_Collider(std::make_shared<T>(std::forward<T>(collider))), m_Mass(mass) {}
        template<ColliderConcept T>
        Physics(T&& collider, float mass, float friction) : m_Collider(std::make_shared<T>(std::forward<T>(collider))), m_Mass(mass), m_Friction(friction) {}
        ~Physics() = default;

        Collider& GetCollider();
        void ApplyForce(glm::vec3 force);

    };
}
