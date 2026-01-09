#include <engine/core/camera.hpp>

namespace Engine
{
    float Camera::GetFarZ() const { return m_FarZ; }
    void Camera::SetFarZ(float farZ) { m_FarZ = glm::clamp(farZ, 100.0f, 1000.0f); }
    float Camera::GetNearZ() const { return m_NearZ; }
    void Camera::SetNearZ(float nearZ) { m_NearZ = glm::clamp(nearZ, 0.1f, 1.0f); }
    float Camera::GetFOV() const { return glm::degrees(m_FOV); }
    void Camera::SetFOV(float degrees) { m_FOV = glm::radians(glm::clamp(degrees, 70.0f, 120.0f)); }
    float Camera::GetPan() { return m_Pan; }
    float Camera::GetTilt() { return m_Tilt; }
    float Camera::GetRoll() { return m_Roll; }
    void Camera::Pan(float degrees)
    {
        m_Pan += glm::radians(degrees);
        if (m_Pan < glm::radians(0.0f)) m_Pan += glm::radians(360.0f);
        if (m_Pan > glm::radians(360.0f)) m_Pan -= glm::radians(360.0f);
    }
    void Camera::Tilt(float degrees) { m_Tilt = glm::clamp(m_Tilt + glm::radians(degrees), glm::radians(-89.0f), glm::radians(89.0f)); }
    void Camera::Roll(float degrees) { m_Roll = glm::clamp(m_Roll + glm::radians(degrees), glm::radians(-89.0f), glm::radians(89.0f)); }
    void Camera::SetPan(float degrees)
    {
        m_Pan = glm::radians(degrees);
        if (m_Pan < glm::radians(0.0f)) m_Pan += glm::radians(360.0f);
        if (m_Pan > glm::radians(360.0f)) m_Pan -= glm::radians(360.0f);
    }
    void Camera::SetTilt(float degrees) { m_Tilt = glm::clamp(glm::radians(degrees), glm::radians(-89.0f), glm::radians(89.0f)); }
    void Camera::SetRoll(float degrees) { m_Roll = glm::clamp(glm::radians(degrees), glm::radians(-89.0f), glm::radians(89.0f)); }
    glm::vec3 Camera::GetUp(const Transform& transform) { return transform.GetOrientation() * glm::quat(glm::vec3(m_Tilt, m_Roll, m_Pan)) * glm::vec3(0, 0, 1); }
    glm::vec3 Camera::GetRight(const Transform& transform) { return transform.GetOrientation() * glm::quat(glm::vec3(m_Tilt, m_Roll, m_Pan)) * glm::vec3(1, 0, 0); }
    glm::vec3 Camera::GetForward(const Transform& transform) { return transform.GetOrientation() * glm::quat(glm::vec3(m_Tilt, m_Roll, m_Pan)) * glm::vec3(0, 1, 0); }
    glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const
    {
        glm::mat4 rotation = glm::mat4_cast(glm::quat(glm::vec3(m_Tilt, m_Roll, m_Pan)));
        glm::mat4 coordinateSwap = glm::mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        return glm::perspective(m_FOV, aspectRatio, m_NearZ, m_FarZ) * coordinateSwap * glm::inverse(rotation);
    }
}
