#pragma once
#include <glm/glm.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Camera : public Component
    {

        private:

        float m_NearZ = 0.1f;
        float m_FarZ = 100.0f;
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
        glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    };
}

// #pragma once

// #include <glm/glm.hpp>
// #include <glm/gtc/quaternion.hpp>

// class camera
// {
//     friend class world;
//     friend class renderer;
//     private:
//         float aspectRatio;
//         float fieldOfView;
//         float nearZ;
//         float farZ;
//         glm::vec3 position;
//         glm::vec3 forward;
//         glm::vec3 right;
//         glm::vec3 up;
//         glm::mat4 translationMatrix;
//         glm::mat4 rotationMatrix;
//         glm::mat4 viewMatrix;
//         glm::mat4 projectionMatrix;
//     public:
//         camera(float aspectRatio = 1, float fieldOfView = 90, float nearZ = 0.1, float farZ = 100, float x = 0, float y = 0, float z = 0);
//         void setFieldOfView(float fieldOfView, bool degrees = true);
//         void setNearClipPlane(float nearZ);
//         void setFarClipPlane(float farZ);
//         void moveTo(float x, float y, float z);
//         void moveBy(float deltaX, float deltaY, float deltaZ);
//         void moveForward(float value);
//         void moveBackward(float value);
//         void moveRight(float value);
//         void moveLeft(float value);
//         void moveUp(float value);
//         void moveDown(float value);
//         void pan(float degrees);
//         void roll(float degrees);
//         void pitch(float degrees);
//         void yaw(float degrees);
//         void update(void);
// };
