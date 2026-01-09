#include <engine/core/physics.hpp>

namespace Engine
{
    float Physics::GetMass() const { return m_Mass; }
    float Physics::GetInverseMass() const { return m_InverseMass; }
    void Physics::SetMass(float mass)
    {
        if (m_Stationary) return;
        m_Mass = mass;
        m_InverseMass = 1 / mass;
        m_CachedInertiaTensor = m_Collider->GetInertiaTensor(mass);
        m_CachedInverseInertiaTensor = glm::inverse(m_CachedInertiaTensor);
    }
    float Physics::GetDrag() const { return m_Drag; }
    void Physics::SetDrag(float drag) { m_Drag = drag; }
    float Physics::GetFriction() const { return m_Friction; }
    void Physics::SetFriction(float friction) { m_Friction = friction; }
    float Physics::GetRestitution() const { return m_Restitution; }
    void Physics::SetRestitution(float restitution) { m_Restitution = restitution; }
    void Physics::ApplyForce(const glm::vec3& force) { m_ForceAccumulator += force; }
    void Physics::ApplyTorque(const glm::vec3& torque) { m_TorqueAccumulator += torque; }
    void Physics::ApplyLinearImpulse(const glm::vec3& impulse) { m_Velocity += impulse * m_InverseMass; }
    void Physics::ApplyAngularImpulse(const glm::vec3& impulse, const glm::mat3& worldInverseInertiaTensor) { m_AngularVelocity += worldInverseInertiaTensor * impulse; }
    void Physics::ResetAccumulators()
    {
        m_ForceAccumulator = glm::vec3(0);
        m_TorqueAccumulator = glm::vec3(0);
    }
    bool Physics::IsStationary() const { return m_Stationary; }
    void Physics::Integrate(float deltaTime, const glm::mat3& worldInverseInertiaTensor)
    {
        m_Velocity += m_ForceAccumulator * m_InverseMass * deltaTime;
        m_Velocity *= 1.0f - m_Drag * deltaTime;

        m_AngularVelocity += worldInverseInertiaTensor * m_TorqueAccumulator * deltaTime;
        m_AngularVelocity *= 1.0f - m_Drag * deltaTime;
    }
    Collider& Physics::GetCollider() const { return *m_Collider; }
    glm::vec3 Physics::GetVelocity() const { return m_Velocity; }
    glm::vec3 Physics::GetAngularVelocity() const { return m_AngularVelocity; }
    glm::mat3 Physics::GetInertiaTensor() const { return m_CachedInertiaTensor; }
    glm::mat3 Physics::GetInverseInertiaTensor() const { return m_CachedInverseInertiaTensor; }
}