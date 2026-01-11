#pragma once
#include <string>
#include <unordered_map>
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
        Handle m_ActiveCameraHandle = entt::null;

        public:

        World() = default;
        ~World() = default;

        Object Create();
        Object Create(const std::string& name);
        Object Get(Handle handle);
        void Remove(Handle handle);
        void Clear();
        template <ComponentConcept... Components>
        auto View() { return m_Registry.view<Components...>().each(); }

        Object GetActiveCamera();
        void SetActiveCamera(Object camera);
        
    };
}