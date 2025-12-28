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
    glm::vec3 Camera::GetUp() { return glm::quat(glm::vec3(m_Tilt, m_Roll, m_Pan)) * glm::vec3(0, 0, 1); }
    glm::vec3 Camera::GetRight() { return glm::quat(glm::vec3(m_Tilt, m_Roll, m_Pan)) * glm::vec3(1, 0, 0); }
    glm::vec3 Camera::GetForward() { return glm::quat(glm::vec3(m_Tilt, m_Roll, m_Pan)) * glm::vec3(0, 1, 0); }
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

// #include <glm/gtc/constants.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// #include <camera.hpp>

// using namespace glm;

// camera::camera(float aspectRatio_, float fieldOfView_, float nearZ_, float farZ_, float x, float y, float z)
// : aspectRatio(aspectRatio_), fieldOfView(radians(fieldOfView_)), nearZ(nearZ_), farZ(farZ_), position(vec3(x, y, z))
// {
//     forward = vec3(1, 0, 0);
//     right = vec3(0, -1, 0);
//     up = vec3(0, 0, 1);
//     translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);
//     rotationMatrix = mat4(right.x, up.x, forward.x, 0, right.y, up.y, forward.y, 0, right.z, up.z, forward.z, 0, 0, 0, 0, 1);
//     viewMatrix = rotationMatrix * translationMatrix;
//     projectionMatrix = mat4(1 / aspectRatio / tan(radians(fieldOfView_) / 2), 0, 0, 0, 0, 1 / tan(radians(fieldOfView_) / 2), 0, 0, 0, 0, (nearZ + farZ) / (farZ - nearZ), 1, 0, 0, 2 * nearZ * farZ / (nearZ - farZ), 0);
// }

// void camera::setFieldOfView(float fieldOfView_, bool degrees)
// {
//     if (degrees) this->fieldOfView = radians(fieldOfView_);
//     else this->fieldOfView = fieldOfView_;
// }
// void camera::setNearClipPlane(float nearZ_) { this->nearZ = nearZ_; }
// void camera::setFarClipPlane(float farZ_) { this->farZ = farZ_; }
// void camera::moveTo(float x, float y, float z) { position = vec3(x, y, z); }
// void camera::moveBy(float deltaX, float deltaY, float deltaZ) { position += vec3(deltaX, deltaY, deltaZ); }
// void camera::moveForward(float value) { position += value * vec3(forward.x, forward.y, 0); }
// void camera::moveBackward(float value) { position += value * vec3(-forward.x, -forward.y, 0); }
// void camera::moveRight(float value) { position += value * vec3(right.x, right.y, 0); }
// void camera::moveLeft(float value) { position += value * vec3(-right.x, -right.y, 0); }
// void camera::moveUp(float value) { position += value * vec3(0, 0, up.z); }
// void camera::moveDown(float value) { position += value * vec3(0, 0, -up.z); }
// void camera::pan(float degrees)
// {
//     quat pan = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * vec3(0, 0, 1));
//     forward = pan * forward * conjugate(pan);
//     right = pan * right * conjugate(pan);
//     up = pan * up * conjugate(pan);
// }
// void camera::roll(float degrees)
// {
//     quat roll = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * forward);
//     right = roll * right * conjugate(roll);
//     up = roll * up * conjugate(roll);
// }
// void camera::pitch(float degrees)
// {
//     quat pitch = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * right);
//     up = pitch * up * conjugate(pitch);
//     forward = pitch * forward * conjugate(pitch);
// }
// void camera::yaw(float degrees)
// {
//     quat yaw = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * up);
//     forward = yaw * forward * conjugate(yaw);
//     right = yaw * right * conjugate(yaw);
// }
// void camera::update(void)
// {
//     translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);
//     rotationMatrix = mat4(right.x, up.x, forward.x, 0, right.y, up.y, forward.y, 0, right.z, up.z, forward.z, 0, 0, 0, 0, 1);
//     viewMatrix = rotationMatrix * translationMatrix;
//     projectionMatrix = mat4(1 / aspectRatio / tan(fieldOfView / 2), 0, 0, 0, 0, 1 / tan(fieldOfView / 2), 0, 0, 0, 0, (nearZ + farZ) / (farZ - nearZ), 1, 0, 0, 2 * nearZ * farZ / (nearZ - farZ), 0);
// }
