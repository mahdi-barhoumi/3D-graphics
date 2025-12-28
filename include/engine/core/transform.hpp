#pragma once
#include <glm/glm.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Transform : public Component
    {
        private:

        glm::vec3 m_Scale = glm::vec3(1);
        glm::vec3 m_Position = glm::vec3(0);
        glm::quat m_Orientation = glm::quat(glm::vec3(0));

        public:

        glm::vec3 Up() const;
        glm::vec3 Right() const;
        glm::vec3 Forward() const;
        glm::mat4 GetWorldMatrix() const;
        glm::mat4 GetInverseWorldMatrix() const;
        void RotateAround(glm::vec3 vector, float degrees);
        void RotateTo(float angleAroundX, float angleAroundY, float angleAroundZ);
        void RotateBy(float deltaAngleAroundX, float deltaAngleAroundY, float deltaAngleAroundZ);
        void ScaleTo(float scaleX, float scaleY, float scaleZ);
        void ScaleBy(float scalarX, float scalarY, float scalarZ);
        void TranslateTo(float x, float y, float z);
        void TranslateBy(float deltaX, float deltaY, float deltaZ);

    };
}
