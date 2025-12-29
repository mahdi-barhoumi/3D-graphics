#include <engine/core/transform.hpp>

namespace Engine
{
    glm::vec3 Transform::GetPosition() const { return m_Position; }
    glm::mat4 Transform::GetScalingMatrix() const { return glm::scale(glm::mat4(1.0f), m_Scale); }
    glm::mat4 Transform::GetInverseScalingMatrix() const { return glm::inverse(GetScalingMatrix()); }
    glm::mat4 Transform::GetRotationMatrix() const { return glm::mat4_cast(m_Orientation); }
    glm::mat4 Transform::GetInverseRotationMatrix() const { return glm::inverse(GetRotationMatrix()); }
    glm::mat4 Transform::GetTranslationMatrix() const { return glm::translate(glm::mat4(1.0f), m_Position); }
    glm::mat4 Transform::GetInverseTranslationMatrix() const { return glm::inverse(GetTranslationMatrix()); }
    glm::mat4 Transform::GetWorldMatrix() const { return GetTranslationMatrix() * GetRotationMatrix() * GetScalingMatrix(); }
    glm::mat4 Transform::GetInverseWorldMatrix() const { return glm::inverse(GetWorldMatrix()); }
    void Transform::TranslateTo(glm::vec3 position) { m_Position = position; }
    void Transform::TranslateBy(glm::vec3 delta) { m_Position += delta; }
    void Transform::TranslateTo(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
    void Transform::TranslateBy(float deltaX, float deltaY, float deltaZ) { m_Position += glm::vec3(deltaX, deltaY, deltaZ); }
    void Transform::RotateAround(glm::vec3 vector, float degrees)
    {
        vector = glm::normalize(vector);
        m_Orientation = glm::normalize(glm::angleAxis(glm::radians(degrees), vector) * m_Orientation);
    }
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
