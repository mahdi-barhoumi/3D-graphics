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
            Box,
            Sphere,
            Capsule,
            Mesh
        };

        Collider() = default;
        ~Collider() = default;
        
        virtual Shape GetShape() const final;
        virtual Vector3 GetLocalSupport(const Vector3& direction) const = 0;
        virtual Vector3 GetWorldSupport(const Transform& transform, const Vector3& direction) const final;
        virtual Vector3 GetLocalInertiaTensor(const Vector3& scale, float mass) const = 0;
        virtual Vector3 GetLocalInverseInertiaTensor(const Vector3& scale, float mass) const final;
        virtual Matrix3 GetWorldInertiaTensor(const Transform& transform, float mass) const final;
        virtual Matrix3 GetWorldInverseInertiaTensor(const Transform& transform, float mass) const final;
        
        protected:

        Shape m_Shape;

    };

    template <typename T>
    concept ColliderConcept = std::derived_from<T, Collider>;

    class BoxCollider : public Collider
    {
        private:
        
        Vector3 m_HalfExtents = Vector3(0.5f);
        
        public:
        
        BoxCollider();
        BoxCollider(float extent);
        BoxCollider(float width, float depth, float height);
        ~BoxCollider() = default;

        Vector3 GetLocalSupport(const Vector3& direction) const override;
        Vector3 GetLocalInertiaTensor(const Vector3& scale, float mass) const override;

    };

    class SphereCollider : public Collider
    {
        private:
        
        float m_Radius = 1.0f;
        
        public:
        
        SphereCollider() = default;
        SphereCollider(float radius);
        ~SphereCollider() = default;

        Vector3 GetLocalSupport(const Vector3& direction) const override;
        Vector3 GetLocalInertiaTensor(const Vector3& scale, float mass) const override;

    };
}
