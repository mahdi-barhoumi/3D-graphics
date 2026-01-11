#include <engine/core/world.hpp>
#include <engine/core/object.hpp>
#include <engine/core/camera.hpp>

namespace Engine
{
    Object World::Get(Handle handle) { return Object(handle, this); }
    Object World::Create()
    {
        Handle handle = m_Registry.create();
        m_Registry.emplace<Object::Metadata>(handle, std::format("Object{}", static_cast<entt::id_type>(handle)));
        return Object(handle, this);
    }
    Object World::Create(const std::string& name)
    {
        Handle handle = m_Registry.create();
        m_Registry.emplace<Object::Metadata>(handle, name);
        return Object(handle, this);
    }
    void World::Clear() { m_Registry.clear(); }
    void World::Remove(Handle handle) { m_Registry.destroy(handle); }

    Object World::GetActiveCamera()
    {
        if (!m_Registry.valid(m_ActiveCameraHandle) || !m_Registry.all_of<Camera>(m_ActiveCameraHandle))
        {
            m_ActiveCameraHandle = entt::null;
            return Object();
        }
        return Object(m_ActiveCameraHandle, this);
    }
    void World::SetActiveCamera(Object camera)
    {
        if (camera.mp_World != this || !m_Registry.valid(camera.m_Handle) || !m_Registry.all_of<Camera>(camera.m_Handle)) return;
        m_ActiveCameraHandle = camera.m_Handle;
    }
}