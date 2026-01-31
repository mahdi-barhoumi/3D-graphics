#pragma once
#include <memory>
#include <engine/core/math.hpp>
#include <engine/core/collider.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Physics : public Component
    {
        public:

        enum class Type
        {
            Stationary,
            Dynamic
        };

        Physics() = default;
        Physics(Type type);
        Physics(float mass);
        ~Physics() = default;

        Type GetType() const;
        bool IsDynamic() const;
        bool IsStationary() const;
        float GetMass() const;
        float GetInverseMass() const;
        void SetMass(float mass);
        float GetDrag() const;
        void SetDrag(float drag);
        float GetFriction() const;
        void SetFriction(float friction);
        float GetRestitution() const;
        void SetRestitution(float restitution);
        Vector3 GetVelocity() const;
        Vector3 GetAngularVelocity() const;
        void ApplyForce(const Vector3& force);
        void ApplyTorque(const Vector3& torque);
        void ApplyLinearImpulse(const Vector3& impulse);
        void ApplyAngularImpulse(const Vector3& impulse, const Matrix3& tensor);
        void Integrate(const Matrix3& tensor, float dt);
        void ResetAccumulators();

        private:

        float m_Mass = 1.0f;
        float m_Drag = 0.4f;
        float m_Friction = 0.3f;
        float m_Restitution = 0.3f;
        float m_InverseMass = 1.0f;
        Vector3 m_Velocity = Vector3(0.0f);
        Vector3 m_ForceAccumulator = Vector3(0.0f);
        Vector3 m_AngularVelocity =  Vector3(0.0f);
        Vector3 m_TorqueAccumulator =  Vector3(0.0f);
        Type m_Type = Type::Dynamic;

    };
}
