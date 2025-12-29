#include <engine/core/physics.hpp>

namespace Engine
{
    Collider& Physics::GetCollider() { return *m_Collider; }
}