#include <engine/core/transform.hpp>

namespace Engine
{
    Transform::Transform(const Vector3& position) : m_Position(position) {}
    Transform::Transform(const Quaternion& orientation) : m_Orientation(orientation) {}
    Transform::Transform(const Vector3& position, const Vector3& scale) : m_Position(position), m_Scale(scale) {}
    Transform::Transform(const Vector3& position, const Quaternion& orientation) : m_Position(position), m_Orientation(orientation) {}
    Transform::Transform(const Vector3& position, const Vector3& scale, const Quaternion& orientation) : m_Position(position), m_Scale(scale), m_Orientation(orientation) {}
    Vector3 Transform::GetPosition() const { return m_Position; }
    Vector3 Transform::GetInversePosition() const { return -m_Position; }
    Vector3 Transform::GetScale() const { return m_Scale; }
    Vector3 Transform::GetInverseScale() const { return Vector3(1.0f / m_Scale.x, 1.0f / m_Scale.y, 1.0f / m_Scale.z); }
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
    Matrix4 Transform::GetRotationMatrix() const { return Matrix4(m_Orientation); }
    Matrix4 Transform::GetInverseRotationMatrix() const { return Matrix4(Conjugated(m_Orientation)); }
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
        Matrix4 result = Matrix4(m_Orientation);
        result[0] *= m_Scale.x;
        result[1] *= m_Scale.y;
        result[2] *= m_Scale.z;
        result[3] = Vector4(m_Position, 1.0f);
        return result;
    }
    Matrix4 Transform::GetInverseWorldMatrix() const
    {
        Quaternion inverseRotation = Conjugated(m_Orientation);
        Matrix4 result = Matrix4(inverseRotation);
        Vector3 inverseScale(1.0f / m_Scale.x, 1.0f / m_Scale.y, 1.0f / m_Scale.z);
        result[0] *= inverseScale.x;
        result[1] *= inverseScale.y;
        result[2] *= inverseScale.z;
        result[3] = Vector4(Rotated(Hadamard(inverseScale, -m_Position), inverseRotation), 1.0f);
        return result;
    }
    void Transform::ScaleTo(const Vector3& scale) { m_Scale = Clamp(scale, Vector3(0.1f), Vector3(1000.0f)); }
    void Transform::ScaleBy(const Vector3& delta) { m_Scale = Clamp(Hadamard(m_Scale, delta), Vector3(0.1f), Vector3(1000.0f)); }
    void Transform::TranslateTo(const Vector3& position) { m_Position = position; }
    void Transform::TranslateBy(const Vector3& delta) { m_Position += delta; }
    void Transform::RotateTo(const Quaternion& orientation) { m_Orientation = Normalized(orientation); }
    void Transform::RotateBy(const Quaternion& delta) { m_Orientation = Normalized(delta * m_Orientation); }
    void Transform::RotateAround(const Vector3& axis, float radians) { m_Orientation = Normalized(Quaternion(Normalized(axis), radians) * m_Orientation); }
}
