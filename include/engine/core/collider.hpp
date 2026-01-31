#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <engine/core/math.hpp>
#include <engine/core/vertex.hpp>
#include <engine/core/transform.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class BoxCollider
    {
        private:
        
        Vector3 m_HalfExtents = Vector3(0.5f);
        
        public:
        
        BoxCollider() = default;
        BoxCollider(float extent);
        BoxCollider(float width, float depth, float height);
        ~BoxCollider() = default;

        Vector3 GetLocalSupport(const Vector3& direction) const;
        Matrix3 GetLocalInertiaTensor(const Vector3& scale, float mass) const;

    };

    class SphereCollider
    {
        private:
        
        float m_Radius = 1.0f;
        
        public:
        
        SphereCollider() = default;
        SphereCollider(float radius);
        ~SphereCollider() = default;

        Vector3 GetLocalSupport(const Vector3& direction) const;
        Matrix3 GetLocalInertiaTensor(const Vector3& scale, float mass) const;

    };

    class CapsuleCollider
    {
        private:
        
        float m_Radius = 1.0f;
        float m_Length = 1.0f;
        
        public:
        
        CapsuleCollider() = default;
        CapsuleCollider(float radius, float length);
        ~CapsuleCollider() = default;

        Vector3 GetLocalSupport(const Vector3& direction) const;
        Matrix3 GetLocalInertiaTensor(const Vector3& scale, float mass) const;

    };

    class MeshCollider
    {
        private:
        
        
        public:
        
        MeshCollider() = default;
        MeshCollider(const std::string& path);
        ~MeshCollider() = default;

        Vector3 GetLocalSupport(const Vector3& direction) const;
        Matrix3 GetLocalInertiaTensor(const Vector3& scale, float mass) const;

    };

    class Collider : public Component
    {
        public:

        enum class Shape
        {
            Box,
            Sphere,
            Capsule,
            Mesh
        };

        Collider() = delete;
        Collider(const BoxCollider& collider);
        Collider(const SphereCollider& collider);
        Collider(const CapsuleCollider& collider);
        Collider(const MeshCollider& collider);
        ~Collider() = default;
        
        Shape GetShape() const;
        Vector3 GetLocalSupport(const Vector3& direction) const;
        Vector3 GetWorldSupport(const Transform& transform, const Vector3& direction) const;
        Matrix3 GetLocalInertiaTensor(const Vector3& scale, float mass) const;
        Matrix3 GetWorldInertiaTensor(const Transform& transform, float mass) const;
        Matrix3 GetLocalInverseInertiaTensor(const Vector3& scale, float mass) const;
        Matrix3 GetWorldInverseInertiaTensor(const Transform& transform, float mass) const;
        
        private:

        Shape m_Shape;
        union
        {
            BoxCollider m_Box;
            SphereCollider m_Sphere;
            CapsuleCollider m_Capsule;
            MeshCollider m_Mesh;
        };

    };
}
