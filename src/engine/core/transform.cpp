#include <engine/core/transform.hpp>

namespace Engine
{
    glm::vec3 Transform::GetPosition() const { return m_Position; }
    glm::quat Transform::GetOrientation() const { return m_Orientation; }
    glm::quat Transform::GetInverseOrientation() const { return glm::conjugate(m_Orientation); }
    glm::mat4 Transform::GetScalingMatrix() const
    { 
        return glm::mat4(
            m_Scale.x, 0.0f,      0.0f,      0.0f,
            0.0f,      m_Scale.y, 0.0f,      0.0f,
            0.0f,      0.0f,      m_Scale.z, 0.0f,
            0.0f,      0.0f,      0.0f,      1.0f
        );
    }
    glm::mat4 Transform::GetInverseScalingMatrix() const
    {
        return glm::mat4(
            1 / m_Scale.x, 0.0f,          0.0f,          0.0f,
            0.0f,          1 / m_Scale.y, 0.0f,          0.0f,
            0.0f,          0.0f,          1 / m_Scale.z, 0.0f,
            0.0f,          0.0f,          0.0f,          1.0f
        );
    }
    glm::mat4 Transform::GetRotationMatrix() const { return glm::mat4_cast(m_Orientation); }
    glm::mat4 Transform::GetInverseRotationMatrix() const { return glm::mat4_cast(glm::conjugate(m_Orientation)); }
    glm::mat4 Transform::GetTranslationMatrix() const
    { 
        return glm::mat4(
            1.0f,         0.0f,         0.0f,         0.0f,
            0.0f,         1.0f,         0.0f,         0.0f,
            0.0f,         0.0f,         1.0f,         0.0f,
            m_Position.x, m_Position.y, m_Position.z, 1.0f
        );
    }
    glm::mat4 Transform::GetInverseTranslationMatrix() const
    {
        return glm::mat4(
             1.0f,          0.0f,          0.0f,         0.0f,
             0.0f,          1.0f,          0.0f,         0.0f,
             0.0f,          0.0f,          1.0f,         0.0f,
            -m_Position.x, -m_Position.y, -m_Position.z, 1.0f
        );
    }
    glm::mat4 Transform::GetWorldMatrix() const
    {
        glm::mat4 result = glm::mat4_cast(m_Orientation);
        result[0] *= m_Scale.x;
        result[1] *= m_Scale.y;
        result[2] *= m_Scale.z;
        result[3] = glm::vec4(m_Position, 1.0f);
        return result;
    }
    glm::mat4 Transform::GetInverseWorldMatrix() const { return GetInverseScalingMatrix() * GetInverseRotationMatrix() * GetInverseTranslationMatrix(); }
    void Transform::TranslateTo(glm::vec3 position) { m_Position = position; }
    void Transform::TranslateBy(glm::vec3 delta) { m_Position += delta; }
    void Transform::TranslateTo(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
    void Transform::TranslateBy(float deltaX, float deltaY, float deltaZ) { m_Position += glm::vec3(deltaX, deltaY, deltaZ); }
    void Transform::RotateAround(glm::vec3 vector, float radians) { m_Orientation = glm::normalize(glm::angleAxis(radians, glm::normalize(vector)) * m_Orientation); }
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
