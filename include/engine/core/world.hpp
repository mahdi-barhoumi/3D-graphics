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
