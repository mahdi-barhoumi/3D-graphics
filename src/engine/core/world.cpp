#include <engine/core/world.hpp>
#include <engine/core/object.hpp>

namespace Engine
{
    Object World::Get(Handle handle) { return Object(handle, this); }
    Handle World::Create() { return m_Registry.create(); }
    void World::Clear() { m_Registry.clear(); }
    void World::Remove(Handle handle) { m_Registry.destroy(handle); }
}

// #include <world.hpp>

// world::world()
// {
// }

// void world::addObject(const object& obj) { objects.push_back(obj); }
// void world::addLight(const light& l) { lights.push_back(l); }
// void world::setCamera(const class camera& cam) { this->camera = cam; }
// void world::load(void)
// {
//     for (unsigned int i = 0; i < objects.size(); i++) objects[i].load();
// }
// void world::unload(void)
// {
//     for (unsigned int i = 0; i < objects.size(); i++) objects[i].unload();
// }
// void world::update(void)
// {
//     for (auto& object : objects) {
//         object.rotate(0.5, 0, 0);
//     }
// }
