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
