#include <engine/core/collider.hpp>

namespace Engine
{
    Vector3 Collider::GetWorldSupport(const Transform& transform, const Vector3& direction) const
    {
        Vector3 localDirection = transform.GetInverseWorldMatrix() * Vector4(direction, 0.0f);
        return transform.GetWorldMatrix() * Vector4(GetSupport(localDirection), 1.0f);
    }
    Matrix3 Collider::GetInverseInertiaTensor(float mass) const { return Inversed(GetInertiaTensor(mass)); }

    CubeCollider::CubeCollider(float length) : m_HalfLength(length * 0.5) { m_Shape = Shape::Cube; }
    Vector3 CubeCollider::GetSupport(const Vector3& direction) const
    {
        return Vector3(
            (direction.x >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.y >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.z >= 0) ? m_HalfLength : -m_HalfLength
        );
    }
    Matrix3 CubeCollider::GetInertiaTensor(float mass) const
    {
        float i = mass * m_HalfLength * m_HalfLength * 2.0f / 3.0f;
        return Matrix3(
               i, 0.0f, 0.0f,
            0.0f,    i, 0.0f,
            0.0f, 0.0f,    i
        );
    }

    PlaneCollider::PlaneCollider(float length) : m_HalfLength(length * 0.5) { m_Shape = Shape::Plane; }
    Vector3 PlaneCollider::GetSupport(const Vector3& direction) const
    {
        return Vector3(
            (direction.x >= 0) ? m_HalfLength : -m_HalfLength,
            (direction.y >= 0) ? m_HalfLength : -m_HalfLength,
            0.0f
        );
    }
    Matrix3 PlaneCollider::GetInertiaTensor(float mass) const
    {
        float ixy = (mass * m_HalfLength * m_HalfLength) / 3.0f;
        return Matrix3(
             ixy,    0.0f,          0.0f,
            0.0f,     ixy,          0.0f,
            0.0f,    0.0f,    2.0f * ixy
        );
    }

    SphereCollider::SphereCollider(float radius) : m_Radius(radius) { m_Shape = Shape::Sphere; }
    Vector3 SphereCollider::GetSupport(const Vector3& direction) const { return Normalized(direction) * m_Radius; }
    Matrix3 SphereCollider::GetInertiaTensor(float mass) const
    {
        float i = (2.0f / 5.0f) * mass * m_Radius * m_Radius;
        return Matrix3(
            i, 0, 0,
            0, i, 0,
            0, 0, i
        );
    }
}
