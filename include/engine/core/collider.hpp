#pragma once
#include <engine/core/math.hpp>
#include <engine/core/transform.hpp>

namespace Engine
{
    class Collider
    {
        public:

        enum class Shape
        {
            Unknown,
            Cube,
            Plane,
            Rectangle,
            Sphere,
            Capsule,
            Mesh
        };

        Collider() = default;
        ~Collider() = default;
        virtual Matrix3 GetInertiaTensor(float mass) const = 0;
        virtual Matrix3 GetInverseInertiaTensor(float mass) const final;
        virtual Vector3 GetSupport(const Vector3& direction) const = 0;
        virtual Vector3 GetWorldSupport(const Transform& transform, const Vector3& direction) const final;
        
        protected:

        Shape m_Shape = Shape::Unknown;

    };

    template <typename T>
    concept ColliderConcept = std::derived_from<T, Collider>;

    class CubeCollider : public Collider
    {
        private:
        
        float m_HalfLength = 0.5;
        
        public:
        
        CubeCollider() = default;
        CubeCollider(float length);
        ~CubeCollider() = default;

        Matrix3 GetInertiaTensor(float mass) const override;
        Vector3 GetSupport(const Vector3& direction) const override;

    };

    class PlaneCollider : public Collider
    {
        private:
        
        float m_HalfLength = 0.5;
        
        public:
        
        PlaneCollider() = default;
        PlaneCollider(float length);
        ~PlaneCollider() = default;

        Matrix3 GetInertiaTensor(float mass) const override;
        Vector3 GetSupport(const Vector3& direction) const override;

    };

    class SphereCollider : public Collider
    {
        private:
        
        float m_Radius = 1;
        
        public:
        
        SphereCollider() = default;
        SphereCollider(float radius);
        ~SphereCollider() = default;

        Matrix3 GetInertiaTensor(float mass) const override;
        Vector3 GetSupport(const Vector3& direction) const override;

    };
}
