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

        bool m_Stationary = true;
        float m_Mass = 1.0f;
        float m_Drag = 0.6f;
        float m_Restitution = 0.5f;
        glm::vec3 m_Velocity = glm::vec3(0);
        glm::vec3 m_Force = glm::vec3(0);
        std::shared_ptr<Collider> m_Collider = nullptr;

        public:

        Physics() = default;
        template<ColliderConcept T>
        Physics(T&& collider, bool stationary = true) : m_Collider(std::make_shared<T>(std::forward<T>(collider))), m_Stationary(stationary) {}
        template<ColliderConcept T>
        Physics(T&& collider, float mass, bool stationary = true) : m_Collider(std::make_shared<T>(std::forward<T>(collider))), m_Mass(mass), m_Stationary(stationary) {}
        template<ColliderConcept T>
        Physics(T&& collider, float mass, float drag, bool stationary = true) : m_Collider(std::make_shared<T>(std::forward<T>(collider))), m_Mass(mass), m_Drag(drag), m_Stationary(stationary) {}
        ~Physics() = default;

        Collider& GetCollider();
        float GetMass();
        void ApplyForce(glm::vec3 force);

    };
}
