#include <engine/core/light.hpp>

namespace Engine
{
    Matrix4 Light::GetProjectionMatrix() const
    {
        const float inverse = 1.0f / (m_NearPlane - m_FarPlane);
        Matrix4 projection(1.0f);
		projection[0][0] = -2.0f / m_Length;
		projection[1][1] = -2.0f / m_Length;
		projection[2][2] = 2.0f * inverse;
		projection[3][2] = (m_FarPlane + m_NearPlane) * inverse;
        return projection;
    }
    Vector3 Light::GetLocalDirection() const { return Vector3(0.0f, 0.0f, -1.0f); }
    Vector3 Light::GetWorldDirection(const Transform& transform) const { return Rotated(Vector3(0.0f, 0.0f, -1.0f), transform.GetOrientation()); }
    Vector3 Light::GetColor() const { return m_Color; }
}