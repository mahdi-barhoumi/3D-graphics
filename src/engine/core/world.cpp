#include <engine/core/world.hpp>
#include <engine/core/object.hpp>

namespace Engine
{
    Object World::Get(Handle handle) { return Object(handle, this); }
    Handle World::Create() { return m_Registry.create(); }
    void World::Clear() { m_Registry.clear(); }
    void World::Remove(Handle handle) { m_Registry.destroy(handle); }
}
