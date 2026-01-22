#pragma once
#include <memory>
#include <engine/core/math.hpp>
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
        Vector3 m_Velocity = Vector3(0.0f);
        Vector3 m_ForceAccumulator = Vector3(0.0f);
        Vector3 m_AngularVelocity =  Vector3(0.0f);
        Vector3 m_TorqueAccumulator =  Vector3(0.0f);
        Matrix3 m_CachedInertiaTensor = Matrix3(1.0f);
        Matrix3 m_CachedInverseInertiaTensor = Matrix3(1.0f);
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
            m_CachedInverseInertiaTensor = Inversed(m_CachedInertiaTensor);
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
            m_CachedInverseInertiaTensor = Inversed(m_CachedInertiaTensor);
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
        void ApplyForce(const Vector3& force);
        void ApplyTorque(const Vector3& torque);
        void ApplyLinearImpulse(const Vector3& impulse);
        void ApplyAngularImpulse(const Vector3& impulse, const Matrix3& worldInverseInertiaTensor);
        void Integrate(float deltaTime, const Matrix3& worldInverseInertiaTensor);
        void ResetAccumulators();
        bool IsStationary() const;
        Collider& GetCollider() const;
        Vector3 GetVelocity() const;
        Vector3 GetAngularVelocity() const;
        Matrix3 GetInertiaTensor() const;
        Matrix3 GetInverseInertiaTensor() const;

    };
}
