#include <engine/core/physics.hpp>

namespace Engine
{
    Physics::Physics(Type type) : m_Type(type)
    {
        if (IsStationary())
        {
            m_Mass = Constants::Infinity;
            m_InverseMass = 0.0f;
        }
    }
    Physics::Physics(float mass)
    {
        m_Type = Type::Dynamic;
        m_Mass = Clamp(mass, 0.1f, 1000.0f);
        m_InverseMass = 1.0f / m_Mass;
    }
    Physics::Type Physics::GetType() const { return m_Type; }
    bool Physics::IsDynamic() const { return m_Type == Type::Dynamic; }
    bool Physics::IsStationary() const { return m_Type == Type::Stationary; }
    float Physics::GetMass() const { return m_Mass; }
    float Physics::GetInverseMass() const { return m_InverseMass; }
    void Physics::SetMass(float mass)
    {
        if (IsStationary()) return;
        m_Mass = mass;
        m_InverseMass = 1.0f / mass;
    }
    float Physics::GetDrag() const { return m_Drag; }
    void Physics::SetDrag(float drag) { m_Drag = Clamp(drag, 0.0f, 1.0f); }
    float Physics::GetFriction() const { return m_Friction; }
    void Physics::SetFriction(float friction) { m_Friction = Clamp(friction, 0.0f, 1.0f); }
    float Physics::GetRestitution() const { return m_Restitution; }
    void Physics::SetRestitution(float restitution) { m_Restitution = Clamp(restitution, 0.0f, 1.0f); }
    void Physics::ApplyForce(const Vector3& force) { m_ForceAccumulator += force; }
    void Physics::ApplyTorque(const Vector3& torque) { m_TorqueAccumulator += torque; }
    void Physics::ApplyLinearImpulse(const Vector3& impulse) { m_Velocity += impulse * m_InverseMass; }
    void Physics::ApplyAngularImpulse(const Vector3& impulse, const Matrix3& tensor) { m_AngularVelocity += tensor * impulse; }
    void Physics::ResetAccumulators()
    {
        m_ForceAccumulator = Vector3(0.0f);
        m_TorqueAccumulator = Vector3(0.0f);
    }
    void Physics::Integrate(const Matrix3& tensor, float dt)
    {
        m_Velocity += m_ForceAccumulator * m_InverseMass * dt;
        m_Velocity *= 1.0f - m_Drag * dt;

        m_AngularVelocity += tensor * m_TorqueAccumulator * dt;
        m_AngularVelocity *= 1.0f - m_Drag * dt;
    }
    Vector3 Physics::GetVelocity() const { return m_Velocity; }
    Vector3 Physics::GetAngularVelocity() const { return m_AngularVelocity; }
}