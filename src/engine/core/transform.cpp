#include <engine/core/transform.hpp>

namespace Engine
{
    glm::vec3 Transform::Up() const { return m_Orientation * glm::vec3(0, 1, 0); }
    glm::vec3 Transform::Right() const { return m_Orientation * glm::vec3(1, 0, 0); }
    glm::vec3 Transform::Forward() const { return m_Orientation * glm::vec3(0, 0, -1); }
    glm::mat4 Transform::GetWorldMatrix() const
    {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
        glm::mat4 rotationMatrix = glm::mat4_cast(m_Orientation);
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), m_Scale);
        return translationMatrix * rotationMatrix * scalingMatrix;
    }
    glm::mat4 Transform::GetInverseWorldMatrix() const { return glm::inverse(GetWorldMatrix()); }
    void Transform::Yaw(float degrees) { RotateAround(Up(), degrees); }
    void Transform::Roll(float degrees) { RotateAround(Forward(), degrees); }
    void Transform::Pitch(float degrees) { RotateAround(Right(), degrees); }
    void Transform::RotateAround(glm::vec3 vector, float degrees) { m_Orientation = glm::normalize(glm::angleAxis(glm::radians(degrees), vector) * m_Orientation); }
    void Transform::TranslateTo(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
    void Transform::TranslateBy(float deltaX, float deltaY, float deltaZ) { m_Position += glm::vec3(deltaX, deltaY, deltaZ); }
    void Transform::RotateTo(float angleAroundX, float angleAroundY, float angleAroundZ)
    {
         m_Orientation = glm::quat(glm::vec3(angleAroundX, angleAroundY, angleAroundZ));
    }
    void Transform::RotateBy(float deltaAngleAroundX, float deltaAngleAroundY, float deltaAngleAroundZ)
    {
        glm::quat delta = glm::quat(glm::vec3(deltaAngleAroundX, deltaAngleAroundY, deltaAngleAroundZ));
        m_Orientation = glm::normalize(delta * m_Orientation);
    }
    void Transform::ScaleTo(float scaleX, float scaleY, float scaleZ) { m_Scale = glm::vec3(scaleX, scaleY, scaleZ); }
    void Transform::ScaleBy(float scalarX, float scalarY, float scalarZ) { m_Scale *= glm::vec3(scalarX, scalarY, scalarZ); }
}

//     forward = vec3(1, 0, 0);
//     right = vec3(0, -1, 0);
//     up = vec3(0, 0, 1);
//     translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);
//     rotationMatrix = mat4(right.x, up.x, forward.x, 0, right.y, up.y, forward.y, 0, right.z, up.z, forward.z, 0, 0, 0, 0, 1);
//     viewMatrix = rotationMatrix * translationMatrix;

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