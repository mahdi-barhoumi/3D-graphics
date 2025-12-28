#include <engine/core/object.hpp>

namespace Engine
{
    Object::Object(Handle handle, World* world) : m_Handle(handle), mp_World(world) {}
    bool Object::IsValid() const { return (mp_World && mp_World->m_Registry.valid(m_Handle)); }
    Handle Object::GetHandle() const { return m_Handle; }
}
