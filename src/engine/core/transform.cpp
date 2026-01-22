#include <engine/core/transform.hpp>

namespace Engine
{
    Vector3 Transform::GetScale() const { return m_Scale; }
    Vector3 Transform::GetPosition() const { return m_Position; }
    Quaternion Transform::GetOrientation() const { return m_Orientation; }
    Quaternion Transform::GetInverseOrientation() const { return Conjugated(m_Orientation); }
    Matrix4 Transform::GetScalingMatrix() const
    { 
        return Matrix4(
            m_Scale.x, 0.0f,      0.0f,      0.0f,
            0.0f,      m_Scale.y, 0.0f,      0.0f,
            0.0f,      0.0f,      m_Scale.z, 0.0f,
            0.0f,      0.0f,      0.0f,      1.0f
        );
    }
    Matrix4 Transform::GetInverseScalingMatrix() const
    {
        return Matrix4(
            1 / m_Scale.x, 0.0f,          0.0f,          0.0f,
            0.0f,          1 / m_Scale.y, 0.0f,          0.0f,
            0.0f,          0.0f,          1 / m_Scale.z, 0.0f,
            0.0f,          0.0f,          0.0f,          1.0f
        );
    }
    Matrix4 Transform::GetRotationMatrix() const { return m_Orientation; }
    Matrix4 Transform::GetInverseRotationMatrix() const { return Conjugated(m_Orientation); }
    Matrix4 Transform::GetTranslationMatrix() const
    { 
        return Matrix4(
            1.0f,         0.0f,         0.0f,         0.0f,
            0.0f,         1.0f,         0.0f,         0.0f,
            0.0f,         0.0f,         1.0f,         0.0f,
            m_Position.x, m_Position.y, m_Position.z, 1.0f
        );
    }
    Matrix4 Transform::GetInverseTranslationMatrix() const
    {
        return Matrix4(
             1.0f,          0.0f,          0.0f,         0.0f,
             0.0f,          1.0f,          0.0f,         0.0f,
             0.0f,          0.0f,          1.0f,         0.0f,
            -m_Position.x, -m_Position.y, -m_Position.z, 1.0f
        );
    }
    Matrix4 Transform::GetWorldMatrix() const
    {
        Matrix4 result = m_Orientation;
        result[0] *= m_Scale.x;
        result[1] *= m_Scale.y;
        result[2] *= m_Scale.z;
        result[3] = Vector4(m_Position, 1.0f);
        return result;
    }
    Matrix4 Transform::GetInverseWorldMatrix() const { return GetInverseScalingMatrix() * GetInverseRotationMatrix() * GetInverseTranslationMatrix(); }
    void Transform::TranslateTo(const Vector3& position) { m_Position = position; }
    void Transform::TranslateBy(const Vector3& delta) { m_Position += delta; }
    void Transform::TranslateTo(float x, float y, float z) { m_Position = Vector3(x, y, z); }
    void Transform::TranslateBy(float deltaX, float deltaY, float deltaZ) { m_Position += Vector3(deltaX, deltaY, deltaZ); }
    void Transform::RotateAround(const Vector3& vector, float radians) { m_Orientation = Normalized(Quaternion(Normalized(vector), radians) * m_Orientation); }
    void Transform::RotateTo(float angleAroundX, float angleAroundY, float angleAroundZ) { m_Orientation = Quaternion(angleAroundX, angleAroundY, angleAroundZ); }
    void Transform::RotateBy(float deltaAngleAroundX, float deltaAngleAroundY, float deltaAngleAroundZ)
    {
        Quaternion delta = Quaternion(deltaAngleAroundX, deltaAngleAroundY, deltaAngleAroundZ);
        m_Orientation = Normalized(delta * m_Orientation);
    }
    void Transform::ScaleTo(float scaleX, float scaleY, float scaleZ) { m_Scale = Vector3(scaleX, scaleY, scaleZ); }
    void Transform::ScaleBy(float scalarX, float scalarY, float scalarZ) { m_Scale = Hadamard(Vector3(scalarX, scalarY, scalarZ), m_Scale); }
}
