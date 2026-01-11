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
        float m_FOV = glm::radians(90.0f);
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
        glm::vec3 GetLocalUp() const;
        glm::vec3 GetLocalRight() const;
        glm::vec3 GetLocalForward() const;
        glm::vec3 GetWorldUp(const glm::quat& orientation) const;
        glm::vec3 GetWorldRight(const glm::quat& orientation) const;
        glm::vec3 GetWorldForward(const glm::quat& orientation) const;
        glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    };
}