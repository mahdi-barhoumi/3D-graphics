#pragma once
#include <engine/core/world.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Object
    {
        friend class World;

        private:

        Handle m_Handle = entt::null;
        World* mp_World;

        Object(Handle handle, World* world);

        public:

        Object() = delete;
        ~Object() = default;
        
        bool IsValid() const;
        Handle GetHandle() const;
        template <ComponentConcept... Components>
        bool Has() const { return mp_World->m_Registry.template all_of<Components...>(m_Handle); }
        template <ComponentConcept C>
        C& Get() { return mp_World->m_Registry.template get<C>(m_Handle); }
        template <ComponentConcept C>
        void Add(const C& component)
        {
            auto& newComponent = mp_World->m_Registry.template emplace<C>(m_Handle);
            newComponent = component;
        }
        template <ComponentConcept C>
        void AddOrReplace(const C& component)
        {
            auto& newComponent = mp_World->m_Registry.template emplace_or_replace<C>(m_Handle);
            newComponent = component;
        }
        template <ComponentConcept C>
        void Remove() { mp_World->m_Registry.template remove<C>(m_Handle); }

    };
}

// #include <glm/glm.hpp>

// #include <mesh.hpp>
// #include <physics.hpp>
// #include <transform.hpp>

// class Object
// {
//     private:
//         Transform transform;
//         Physics* physics;
//         const Mesh* mesh;

//     public:
//         Object(const Mesh* mesh);
//         Object(const Mesh* mesh, const Physics& physics);
//         ~Object();

//         const Mesh& getMesh(void);

//         bool hasPhysics(void);
//         Physics& getPhysics(void);

//         Transform& getTransform(void);
//         void moveTo(float x, float y, float z);
//         void moveBy(float deltaX, float deltaY, float deltaZ);
//         void setRotation(float degreesAroundX, float degreesAroundY, float degreesAroundZ);
//         void setRotationRadians(float radiansAroundX, float radiansAroundY, float radiansAroundZ);
//         void rotateAroundX(float degrees);
//         void rotateAroundY(float degrees);
//         void rotateAroundZ(float degrees);
//         void rotateAroundXRadians(float radians);
//         void rotateAroundYRadians(float radians);
//         void rotateAroundZRadians(float radians);
//         void setScale(float scalarX, float scalarY, float scalarZ);

//         virtual void update(void);
// };
