#include <stdexcept>
#include <engine/core/collider.hpp>

namespace Engine
{
    Collider::Collider(const BoxCollider& collider) : m_Shape(Shape::Box), m_Box(collider) {}
    Collider::Collider(const SphereCollider& collider) : m_Shape(Shape::Sphere), m_Sphere(collider) {}
    Collider::Collider(const CapsuleCollider& collider) : m_Shape(Shape::Capsule), m_Capsule(collider) {}
    Collider::Collider(const MeshCollider& collider) : m_Shape(Shape::Mesh), m_Mesh(collider) {}
    Collider::Shape Collider::GetShape() const { return m_Shape; }
    Vector3 Collider::GetLocalSupport(const Vector3& direction) const
    {
        switch (m_Shape)
        {
            case Shape::Box: return m_Box.GetLocalSupport(direction);
            case Shape::Sphere: return m_Sphere.GetLocalSupport(direction);
            // case Shape::Capsule: return m_Capsule.GetLocalSupport(direction);
            // case Shape::Mesh: return m_Mesh.GetLocalSupport(direction);
            default: throw std::runtime_error("Invalid collider shape.");
        }
    }
    Vector3 Collider::GetWorldSupport(const Transform& transform, const Vector3& direction) const
    {
        Vector3 localDirection = transform.GetInverseWorldMatrix() * direction;
        return transform.GetWorldMatrix() * Vector4(GetLocalSupport(localDirection), 1.0f);
    }
    Matrix3 Collider::GetLocalInertiaTensor(const Vector3& scale, float mass) const
    {
        switch (m_Shape)
        {
            case Shape::Box: return m_Box.GetLocalInertiaTensor(scale, mass);
            case Shape::Sphere: return m_Sphere.GetLocalInertiaTensor(scale, mass);
            // case Shape::Capsule: return m_Capsule.GetLocalInertiaTensor(scale, mass);
            // case Shape::Mesh: return m_Mesh.GetLocalInertiaTensor(scale, mass);
            default: throw std::runtime_error("Invalid collider shape.");
        }
    }
    Matrix3 Collider::GetWorldInertiaTensor(const Transform& transform, float mass) const
    {
        const Matrix3 rotationMatrix = transform.GetRotationMatrix();
        return rotationMatrix * Matrix3(GetLocalInertiaTensor(transform.GetScale(), mass)) * Transposed(rotationMatrix);
    }
    Matrix3 Collider::GetLocalInverseInertiaTensor(const Vector3& scale, float mass) const
    {
        Matrix3 tensor = GetLocalInertiaTensor(scale, mass);
        tensor[0][0] = 1.0f / tensor[0][0];
        tensor[1][1] = 1.0f / tensor[1][1];
        tensor[2][2] = 1.0f / tensor[2][2];
        return tensor;
    }
    Matrix3 Collider::GetWorldInverseInertiaTensor(const Transform& transform, float mass) const
    {
        Matrix3 rotationMatrix = transform.GetRotationMatrix();
        return rotationMatrix * Matrix3(GetLocalInverseInertiaTensor(transform.GetScale(), mass)) * Transposed(rotationMatrix);
    }

    BoxCollider::BoxCollider(float length) : m_HalfExtents(0.5f * length) {}
    BoxCollider::BoxCollider(float width, float depth, float height) : m_HalfExtents(0.5f * width, 0.5f * depth, 0.5f * height) {}
    Vector3 BoxCollider::GetLocalSupport(const Vector3& direction) const
    {
        return Vector3(
            (direction.x >= 0) ? m_HalfExtents.x : -m_HalfExtents.x,
            (direction.y >= 0) ? m_HalfExtents.y : -m_HalfExtents.y,
            (direction.z >= 0) ? m_HalfExtents.z : -m_HalfExtents.z
        );
    }
    Matrix3 BoxCollider::GetLocalInertiaTensor(const Vector3& scale, float mass) const
    {
        const Vector3 h = Hadamard(m_HalfExtents, scale);
        float common = mass / 3.0f;
        return Vector3(
            common * (h.y * h.y + h.z * h.z),
            common * (h.x * h.x + h.z * h.z),
            common * (h.x * h.x + h.y * h.y)
        );
    }

    SphereCollider::SphereCollider(float radius) : m_Radius(radius) {}
    Vector3 SphereCollider::GetLocalSupport(const Vector3& direction) const { return Normalized(direction) * m_Radius; }
    Matrix3 SphereCollider::GetLocalInertiaTensor(const Vector3& scale, float mass) const
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
