#include <engine/core/collider.hpp>

namespace Engine
{
    glm::vec3 Collider::GetWorldSupport(const Transform& transform, const glm::vec3& direction) const
    {
        glm::vec3 localDirection = transform.GetInverseWorldMatrix() * glm::vec4(direction, 0.0f);
        return transform.GetWorldMatrix() * glm::vec4(GetSupport(localDirection), 1.0f);
    }
    glm::mat3 Collider::GetInverseInertiaTensor(float mass) const { return glm::inverse(GetInertiaTensor(mass)); }

    CubeCollider::CubeCollider(float length) : m_HalfLength(length * 0.5) { m_Shape = Shape::Cube; }
    glm::vec3 CubeCollider::GetSupport(const glm::vec3& direction) const
    {
        return glm::vec3(
            (direction.x >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.y >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.z >= 0) ? m_HalfLength : -m_HalfLength
        );
    }
    glm::mat3 CubeCollider::GetInertiaTensor(float mass) const
    {
        float i = mass * m_HalfLength * m_HalfLength * 2.0f / 3.0f;
        return glm::mat3(
            i,    0.0f, 0.0f,
            0.0f, i,    0.0f,
            0.0f, 0.0f, i
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
    glm::mat3 PlaneCollider::GetInertiaTensor(float mass) const
    {
        float ixy = (mass * m_HalfLength * m_HalfLength) / 3.0f;
        return glm::mat3(
            ixy, 0,   0,
            0,   ixy, 0,
            0,   0,   ixy * 2.0f
        );
    }

    SphereCollider::SphereCollider(float radius) : m_Radius(radius) { m_Shape = Shape::Sphere; }
    glm::vec3 SphereCollider::GetSupport(const glm::vec3& direction) const { return glm::normalize(direction) * m_Radius; }
    glm::mat3 SphereCollider::GetInertiaTensor(float mass) const
    {
        float i = (2.0f / 5.0f) * mass * m_Radius * m_Radius;
        return glm::mat3(
            i, 0, 0,
            0, i, 0,
            0, 0, i
        );
    }
}
