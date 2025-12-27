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

        Transform() = default;
        Transform(const Transform&) = default;
        ~Transform() = default;

        void TranslateTo(float x, float y, float z);
        void TranslateBy(float deltaX, float deltaY, float deltaZ);
        void RotateTo(float angleAroundX, float angleAroundY, float angleAroundZ);
        void RotateBy(float deltaAngleAroundX, float deltaAngleAroundY, float deltaAngleAroundZ);
        void ScaleTo(float scaleX, float scaleY, float scaleZ);
        void ScaleBy(float scalarX, float scalarY, float scalarZ);
        glm::mat4 GetWorldMatrix() const;
        glm::mat4 GetInverseWorldMatrix() const;

    };
}

// #include <glm/glm.hpp>

// class Transform 
// {
//     private:
//         glm::vec3 scale = glm::vec3(1);
//         glm::vec3 position = glm::vec3(0);
//         glm::quat rotation = glm::quat(glm::vec3(0));
//         glm::mat4 scalingMatrix = mat4(1);
//         glm::mat4 rotationMatrix = mat4(1);
//         glm::mat4 translationMatrix = mat4(1);
//         glm::mat4 modelMatrix = mat4(1);

//         void calculateScalingMatrix(void);
//         void calculateRotationMatrix(void);
//         void calculateTranslationMatrix(void);
//         void calculateModelMatrix(void);

//     public:
//         Transform() = default;
//         Transform(const glm::vec3& scale);
//         Transform(const glm::vec3& scale, const glm::vec3& position);
//         Transform(const glm::vec3& scale, const glm::vec3& position, const glm::quat& rotation);

//         const glm::vec3& getScale(void);
//         void setScale(const glm::vec3& scale);
//         const glm::vec3& getPosition(void);
//         void setPosition(const glm::vec3& position);
//         const glm::quat& getRotation(void);
//         void setRotation(const glm::quat& rotation);
//         const glm::mat4& getScalingMatrix(void);
//         const glm::mat4& getRotationMatrix(void);
//         const glm::mat4& getTranslationMatrix(void);
//         const glm::mat4& getModelMatrix(void);
// };