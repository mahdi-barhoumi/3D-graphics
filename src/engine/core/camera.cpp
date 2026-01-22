#include <engine/core/camera.hpp>

namespace Engine
{
    Camera::Camera(float FOV, float nearZ, float farZ) : m_FOV(Radians(Clamp(FOV, 75.0f, 120.0f))), m_NearZ(Clamp(nearZ, 0.1f, 1.0f)), m_FarZ(Clamp(farZ, 100.0f, 1000.0f)) {}
    float Camera::GetFOV() const { return Degrees(m_FOV); }
    void Camera::SetFOV(float degrees) { m_FOV = Radians(Clamp(degrees, 75.0f, 120.0f)); }
    float Camera::GetNearZ() const { return m_NearZ; }
    void Camera::SetNearZ(float nearZ) { m_NearZ = Clamp(nearZ, 0.1f, 1.0f); }
    float Camera::GetFarZ() const { return m_FarZ; }
    void Camera::SetFarZ(float farZ) { m_FarZ = Clamp(farZ, 100.0f, 1000.0f); }
    float Camera::GetPan() { return Degrees(m_Pan); }
    void Camera::SetPan(float degrees)
    {
        m_Pan = Radians(degrees);
        if (m_Pan < Radians(0.0f)) m_Pan += Radians(360.0f);
        else if (m_Pan > Radians(360.0f)) m_Pan -= Radians(360.0f);
    }
    void Camera::Pan(float degrees)
    {
        m_Pan += Radians(degrees);
        if (m_Pan < Radians(0.0f)) m_Pan += Radians(360.0f);
        else if (m_Pan > Radians(360.0f)) m_Pan -= Radians(360.0f);
    }
    float Camera::GetTilt() { return Degrees(m_Tilt); }
    void Camera::SetTilt(float degrees) { m_Tilt = Clamp(Radians(degrees), Radians(-90.0f), Radians(90.0f)); }
    void Camera::Tilt(float degrees) { m_Tilt = Clamp(m_Tilt + Radians(degrees), Radians(-90.0f), Radians(90.0f)); }
    float Camera::GetRoll() { return Degrees(m_Roll); }
    void Camera::SetRoll(float degrees) { m_Roll = Clamp(Radians(degrees), Radians(-90.0f), Radians(90.0f)); }
    void Camera::Roll(float degrees) { m_Roll = Clamp(m_Roll + Radians(degrees), Radians(-90.0f), Radians(90.0f)); }
    Vector3 Camera::GetLocalUp() const { return Rotated(Vector3(0.0f, 0.0f, 1.0f), Quaternion(m_Tilt, m_Roll, m_Pan)); }
    Vector3 Camera::GetLocalRight() const { return Rotated(Vector3(1.0f, 0.0f, 0.0f), Quaternion(m_Tilt, m_Roll, m_Pan));  }
    Vector3 Camera::GetLocalForward() const { return Rotated(Vector3(0.0f, 1.0f, 0.0f), Quaternion(m_Tilt, m_Roll, m_Pan)); }
    Vector3 Camera::GetWorldUp(const Quaternion& orientation) const { return Rotated(Vector3(0.0f, 0.0f, 1.0f), orientation * Quaternion(m_Tilt, m_Roll, m_Pan)); }
    Vector3 Camera::GetWorldRight(const Quaternion& orientation) const { return Rotated(Vector3(1.0f, 0.0f, 0.0f), orientation * Quaternion(m_Tilt, m_Roll, m_Pan)); }
    Vector3 Camera::GetWorldForward(const Quaternion& orientation) const { return Rotated(Vector3(0.0f, 1.0f, 0.0f), orientation * Quaternion(m_Tilt, m_Roll, m_Pan)); }
    Matrix4 Camera::GetProjectionMatrix(float aspectRatio) const
    {
        Matrix4 inverseRotation = Conjugated(Quaternion(m_Tilt, m_Roll, m_Pan));
        Matrix4 coordinateSwap = Matrix4(
            1.0f,  0.0f,  0.0f,  0.0f,
            0.0f,  0.0f, -1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,  0.0f,
            0.0f,  0.0f,  0.0f,  1.0f
        );

		const float tanHalfFOV = Tan(m_FOV * 0.5f);
		Matrix4 projection(0.0f);
		projection[0][0] = 1.0f / (aspectRatio * tanHalfFOV);
		projection[1][1] = 1.0f / (tanHalfFOV);
		projection[2][2] = - (m_FarZ + m_NearZ) / (m_FarZ - m_NearZ);
		projection[2][3] = - 1.0f;
		projection[3][2] = - 2.0f * m_FarZ * m_NearZ / (m_FarZ - m_NearZ);

        return projection * coordinateSwap * inverseRotation;
    }
}