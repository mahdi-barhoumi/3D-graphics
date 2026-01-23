#include <engine/core/physics.hpp>

namespace Engine
{
    float Physics::GetMass() const { return m_Mass; }
    float Physics::GetInverseMass() const { return m_InverseMass; }
    void Physics::SetMass(float mass)
    {
        if (m_Stationary) return;
        m_Mass = mass;
        m_InverseMass = 1.0f / mass;
    }
    float Physics::GetDrag() const { return m_Drag; }
    void Physics::SetDrag(float drag) { m_Drag = drag; }
    float Physics::GetFriction() const { return m_Friction; }
    void Physics::SetFriction(float friction) { m_Friction = friction; }
    float Physics::GetRestitution() const { return m_Restitution; }
    void Physics::SetRestitution(float restitution) { m_Restitution = restitution; }
    void Physics::ApplyForce(const Vector3& force) { m_ForceAccumulator += force; }
    void Physics::ApplyTorque(const Vector3& torque) { m_TorqueAccumulator += torque; }
    void Physics::ApplyLinearImpulse(const Vector3& impulse) { m_Velocity += impulse * m_InverseMass; }
    void Physics::ApplyAngularImpulse(const Vector3& impulse, const Matrix3& tensor) { m_AngularVelocity += tensor * impulse; }
    void Physics::ResetAccumulators()
    {
        m_ForceAccumulator = Vector3(0.0f);
        m_TorqueAccumulator = Vector3(0.0f);
    }
    bool Physics::IsStationary() const { return m_Stationary; }
    void Physics::Integrate(const Matrix3& tensor, float dt)
    {
        m_Velocity += m_ForceAccumulator * m_InverseMass * dt;
        m_Velocity *= 1.0f - m_Drag * dt;

        m_AngularVelocity += tensor * m_TorqueAccumulator * dt;
        m_AngularVelocity *= 1.0f - m_Drag * dt;
    }
    Collider& Physics::GetCollider() const { return *m_Collider; }
    Vector3 Physics::GetVelocity() const { return m_Velocity; }
    Vector3 Physics::GetAngularVelocity() const { return m_AngularVelocity; }
}