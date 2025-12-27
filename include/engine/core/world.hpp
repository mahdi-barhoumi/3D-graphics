#pragma once
#include <entt/entt.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    using Handle = entt::entity;

    class Object;

    class World
    {
        friend class Object;

        private:

        entt::registry m_Registry;

        public:

        World() = default;
        ~World() = default;

        Object Get(Handle handle);
        Handle Create();
        void Clear();
        void Remove(Handle handle);
        template <ComponentConcept... Components>
        auto View() { return m_Registry.view<Components...>().each(); }
        
    };
}

// #pragma once

// #include <vector>

// #include <object.hpp>
// #include <light.hpp>
// #include <camera.hpp>

// class world
// {
//     friend class renderer;
//     private:
//         camera camera;
//         std::vector<object> objects;
//         std::vector<light> lights;
//     public:
//         world();
//         void addObject(const object& object);
//         void addLight(const light& light);
//         void setCamera(const class camera& camera);
//         void load(void);
//         void unload(void);
//         void update(void);
// };
