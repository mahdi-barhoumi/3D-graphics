#include <engine/core/light.hpp>

namespace Engine
{
    glm::mat4 Light::GetProjectionMatrix() const
    {
        return glm::ortho(- m_Length * 0.5f, m_Length * 0.5f, - m_Length * 0.5f, m_Length * 0.5f, m_NearPlane, m_FarPlane);
    }
    glm::vec3 Light::GetDirection(const Transform& transform) const { return transform.GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f); }
}