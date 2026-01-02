#pragma once
#include <glm/glm.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Camera : public Component
    {

        private:

        float m_Pan = 0.0f;
        float m_Tilt = 0.0f;
        float m_Roll = 0.0f;
        float m_NearZ = 0.1f;
        float m_FarZ = 1000.0f;
        float m_FOV = glm::radians(90.0f);

        public:

        Camera() = default;
        Camera(const Camera&) = default;
        ~Camera() = default;

        float GetFarZ() const;
        void SetFarZ(float farZ);
        float GetNearZ() const;
        void SetNearZ(float nearZ);
        float GetFOV() const;
        void SetFOV(float degrees);
        float GetPan();
        float GetTilt();
        float GetRoll();
        void Pan(float degrees);
        void Tilt(float degrees);
        void Roll(float degrees);
        void SetPan(float degrees);
        void SetTilt(float degrees);
        void SetRoll(float degrees);
        glm::vec3 GetUp();
        glm::vec3 GetRight();
        glm::vec3 GetForward();
        glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    };
}
