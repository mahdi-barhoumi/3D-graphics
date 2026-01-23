#include <engine/core/collider.hpp>

namespace Engine
{
    Collider::Shape Collider::GetShape() const { return m_Shape; }
    Vector3 Collider::GetWorldSupport(const Transform& transform, const Vector3& direction) const
    {
        Vector3 localDirection = transform.GetInverseWorldMatrix() * direction;
        return transform.GetWorldMatrix() * Vector4(GetLocalSupport(localDirection), 1.0f);
    }
    Vector3 Collider::GetLocalInverseInertiaTensor(const Vector3& scale, float mass) const
    {
        const Vector3 tensor = GetLocalInertiaTensor(scale, mass);
        return Vector3(1.0f / tensor.x, 1.0f / tensor.y, 1.0f / tensor.z);
    }
    Matrix3 Collider::GetWorldInertiaTensor(const Transform& transform, float mass) const
    {
        const Matrix3 rotationMatrix = transform.GetRotationMatrix();
        return rotationMatrix * Matrix3(GetLocalInertiaTensor(transform.GetScale(), mass)) * Transposed(rotationMatrix);
    }
    Matrix3 Collider::GetWorldInverseInertiaTensor(const Transform& transform, float mass) const
    {
        Matrix3 rotationMatrix = transform.GetRotationMatrix();
        return rotationMatrix * Matrix3(GetLocalInverseInertiaTensor(transform.GetScale(), mass)) * Transposed(rotationMatrix);
    }

    BoxCollider::BoxCollider() { m_Shape = Shape::Box; }
    BoxCollider::BoxCollider(float length) : m_HalfExtents(0.5f * length) { m_Shape = Shape::Box; }
    BoxCollider::BoxCollider(float width, float depth, float height) : m_HalfExtents(0.5f * width, 0.5f * depth, 0.5f * height) { m_Shape = Shape::Box; }
    Vector3 BoxCollider::GetLocalSupport(const Vector3& direction) const
    {
        return Vector3(
            (direction.x >= 0) ? m_HalfExtents.x : -m_HalfExtents.x,
            (direction.y >= 0) ? m_HalfExtents.y : -m_HalfExtents.y,
            (direction.z >= 0) ? m_HalfExtents.z : -m_HalfExtents.z
        );
    }
    Vector3 BoxCollider::GetLocalInertiaTensor(const Vector3& scale, float mass) const
    {
        const Vector3 h = Hadamard(m_HalfExtents, scale);
        float common = mass / 3.0f;
        return Vector3(
            common * (h.y * h.y + h.z * h.z),
            common * (h.x * h.x + h.z * h.z),
            common * (h.x * h.x + h.y * h.y)
        );
    }

    SphereCollider::SphereCollider(float radius) : m_Radius(radius) { m_Shape = Shape::Sphere; }
    Vector3 SphereCollider::GetLocalSupport(const Vector3& direction) const { return Normalized(direction) * m_Radius; }
    Vector3 SphereCollider::GetLocalInertiaTensor(const Vector3& scale, float mass) const
    {
        Vector3 h = m_Radius * scale;
        h = Hadamard(h, h);
        const float fraction = mass / 5.0f;
        const float ix = fraction * (h.y + h.z);
        const float iy = fraction * (h.x + h.z);
        const float iz = fraction * (h.x + h.y);
        return Vector3(ix, iy, iz);
    }
}
