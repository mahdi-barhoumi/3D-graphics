#include <engine/core/collider.hpp>

namespace Engine
{
    glm::vec3 Collider::GetWorldSupport(const Transform& transform, const glm::vec3& direction) const
    {
        // Transpose if we add non uniform scaling vvv
        glm::vec3 localDirection = transform.GetInverseWorldMatrix() * glm::vec4(glm::normalize(direction), 0.0f);
        glm::normalize(localDirection);
        return transform.GetWorldMatrix() * glm::vec4(GetSupport(localDirection), 1.0f);
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

    SphereCollider::SphereCollider(float radius) : m_Radius(radius) { m_Shape = Shape::Sphere; }
    glm::vec3 SphereCollider::GetSupport(const glm::vec3& direction) const
    {
        return glm::normalize(direction) * m_Radius;
    }
}
