#include <engine/core/collider.hpp>

namespace Engine
{
    glm::vec3 Collider::GetWorldSupport(const Transform& transform, const glm::vec3& direction) const
    {
        glm::vec3 local = transform.GetInverseScalingMatrix() * transform.GetInverseRotationMatrix() * glm::vec4(direction, 0.0f);
        glm::vec3 support = GetSupport(local);
        return transform.GetWorldMatrix() * glm::vec4(support, 1.0f);
    }
    
    CubeCollider::CubeCollider(float length) : m_HalfLength(length * 0.5) { m_Shape = Shape::Cube; }
    glm::vec3 CubeCollider::GetSupport(const glm::vec3& direction) const
    {
        return glm::vec3(
            (direction.x >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.y >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.z >= 0) ? m_HalfLength : -m_HalfLength
        );
    }

    PlaneCollider::PlaneCollider(float length) : m_HalfLength(length * 0.5) { m_Shape = Shape::Plane; }
    glm::vec3 PlaneCollider::GetSupport(const glm::vec3& direction) const
    {
        return glm::vec3(
            (direction.x >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.y >= 0) ? m_HalfLength : -m_HalfLength,
            0.0f
        );
    }
}
