#pragma once
#include <engine/core/math.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Camera : public Component
    {
        private:

        float m_Pan = 0.0f;
        float m_Tilt = 0.0f;
        float m_Roll = 0.0f;
        float m_FOV = Degrees(90.0f);
        float m_NearZ = 0.1f;
        float m_FarZ = 1000.0f;

        public:

        Camera(float FOV = 90.0f, float nearZ = 0.1f, float farZ = 1000.0f);
        Camera(const Camera& other) = default;
        ~Camera() = default;

        float GetFOV() const;
        void SetFOV(float degrees);
        float GetNearZ() const;
        void SetNearZ(float nearZ);
        float GetFarZ() const;
        void SetFarZ(float farZ);
        float GetPan();
        void SetPan(float degrees);
        void Pan(float degrees);
        float GetTilt();
        void SetTilt(float degrees);
        void Tilt(float degrees);
        float GetRoll();
        void SetRoll(float degrees);
        void Roll(float degrees);
        Vector3 GetLocalUp() const;
        Vector3 GetLocalRight() const;
        Vector3 GetLocalForward() const;
        Vector3 GetWorldUp(const Quaternion& orientation) const;
        Vector3 GetWorldRight(const Quaternion& orientation) const;
        Vector3 GetWorldForward(const Quaternion& orientation) const;
        Matrix4 GetProjectionMatrix(float aspectRatio) const;

    };
}