#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <engine/core/collider.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Physics : public Component
    {
        private:

        float m_Mass = 1.0f;
        float m_Drag = 0.4f;
        float m_Friction = 0.3f;
        float m_Restitution = 0.3f;
        float m_InverseMass = 1.0f;
        bool m_Stationary = false;
        glm::vec3 m_Velocity = glm::vec3(0);
        glm::vec3 m_ForceAccumulator = glm::vec3(0);
        glm::vec3 m_AngularVelocity =  glm::vec3(0);
        glm::vec3 m_TorqueAccumulator =  glm::vec3(0);
        glm::mat3 m_CachedInertiaTensor = glm::mat4(1);
        glm::mat3 m_CachedInverseInertiaTensor = glm::mat4(1);
        std::shared_ptr<Collider> m_Collider = nullptr;

        public:

        Physics() = default;
        template<ColliderConcept T>
        Physics(T&& collider, bool stationary = false) : m_Collider(std::make_shared<T>(std::forward<T>(collider))), m_Stationary(stationary)
        {
            if (m_Stationary)
            {
                m_Mass = std::numeric_limits<float>::infinity();
                m_InverseMass = 0;
            }
            m_CachedInertiaTensor = m_Collider->GetInertiaTensor(m_Mass);
            m_CachedInverseInertiaTensor = glm::inverse(m_CachedInertiaTensor);
        }
        template<ColliderConcept T>
        Physics(T&& collider, float mass, bool stationary = false) : m_Collider(std::make_shared<T>(std::forward<T>(collider))), m_Mass(mass), m_InverseMass(1 / mass), m_Stationary(stationary)
        {
            if (m_Stationary)
            {
                m_Mass = std::numeric_limits<float>::infinity();
                m_InverseMass = 0;
            }
            m_CachedInertiaTensor = m_Collider->GetInertiaTensor(m_Mass);
            m_CachedInverseInertiaTensor = glm::inverse(m_CachedInertiaTensor);
        }
        ~Physics() = default;

        float GetMass() const;
        float GetInverseMass() const;
        void SetMass(float mass);
        float GetDrag() const;
        void SetDrag(float drag);
        float GetFriction() const;
        void SetFriction(float friction);
        float GetRestitution() const;
        void SetRestitution(float restitution);
        void ApplyForce(const glm::vec3& force);
        void ApplyTorque(const glm::vec3& torque);
        void ApplyLinearImpulse(const glm::vec3& impulse);
        void ApplyAngularImpulse(const glm::vec3& impulse, const glm::mat3& worldInverseInertiaTensor);
        void Integrate(float deltaTime, const glm::mat3& worldInverseInertiaTensor);
        void ResetAccumulators();
        bool IsStationary() const;
        Collider& GetCollider() const;
        glm::vec3 GetVelocity() const;
        glm::vec3 GetAngularVelocity() const;
        glm::mat3 GetInertiaTensor() const;
        glm::mat3 GetInverseInertiaTensor() const;

    };
}
