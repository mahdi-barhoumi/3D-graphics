#include <engine/core/physics.hpp>

namespace Engine
{
    Collider& Physics::GetCollider() { return *m_Collider; }
    float Physics::GetMass() { return m_Mass; }
    void Physics::ApplyForce(glm::vec3 force) { m_Force += force; }
    bool Physics::IsStationary() { return m_Stationary; }
}