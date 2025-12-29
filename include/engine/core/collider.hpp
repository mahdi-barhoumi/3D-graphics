#pragma once
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
            Rectangle,
            Sphere,
            Capsule,
            Mesh
        };

        Collider() = default;
        ~Collider() = default;

        virtual glm::vec3 GetSupport(const glm::vec3& direction) const = 0;
        virtual glm::vec3 GetWorldSupport(const Transform& transform, const glm::vec3& direction) const final;

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

        glm::vec3 GetSupport(const glm::vec3& direction) const override;

    };
}
