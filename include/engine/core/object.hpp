#pragma once
#include <string>
#include <engine/core/world.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Object
    {
        friend class World;

        private:

        struct Metadata : public Component
        {
            std::string name;

            Metadata() = default;
            Metadata(const Metadata& other) = default;
            Metadata(const std::string& name) : name(name) {}
            ~Metadata() = default;
        };

        World* mp_World = nullptr;
        Handle m_Handle = entt::null;

        Object(Handle handle, World* world);

        public:

        Object() = default;
        ~Object() = default;
        
        bool IsValid() const;
        Handle GetHandle() const;
        std::string GetName() const;
        void SetName(const std::string& name);
        template <ComponentConcept... Components>
        bool Has() const { return mp_World->m_Registry.template all_of<Components...>(m_Handle); }
        template <ComponentConcept C>
        C& Get() { return mp_World->m_Registry.template get<C>(m_Handle); }
        template <ComponentConcept C>
        const C& Get() const { return mp_World->m_Registry.template get<C>(m_Handle); }
        template <ComponentConcept C>
        void Add(const C& component) { mp_World->m_Registry.template emplace<C>(m_Handle, component); }
        template <ComponentConcept C, typename... Arguments>
        void Add(Arguments&&... arguments) { mp_World->m_Registry.template emplace<C>(m_Handle, std::forward<Arguments>(arguments)...); }
        template <ComponentConcept C>
        void AddOrReplace(const C& component) { mp_World->m_Registry.template emplace_or_replace<C>(m_Handle, component); }
        template <ComponentConcept C, typename... Arguments>
        void AddOrReplace(Arguments&&... arguments) { mp_World->m_Registry.template emplace_or_replace<C>(m_Handle, std::forward<Arguments>(arguments)...); }
        template <ComponentConcept C>
        void Remove() { mp_World->m_Registry.template remove<C>(m_Handle); }

    };
}