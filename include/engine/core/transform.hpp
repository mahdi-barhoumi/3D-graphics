#pragma once
#include <engine/core/math.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Transform : public Component
    {
        public:

        Vector3 m_Scale = Vector3(1.0f);
        Vector3 m_Position = Vector3(0.0f);
        Quaternion m_Orientation = Quaternion(0.0f, 0.0f, 0.0f);

        public:

        Vector3 GetScale() const;
        Vector3 GetPosition() const;
        Quaternion GetOrientation() const;
        Quaternion GetInverseOrientation() const;
        Matrix4 GetScalingMatrix() const;
        Matrix4 GetInverseScalingMatrix() const;
        Matrix4 GetRotationMatrix() const;
        Matrix4 GetInverseRotationMatrix() const;
        Matrix4 GetTranslationMatrix() const;
        Matrix4 GetInverseTranslationMatrix() const;
        Matrix4 GetWorldMatrix() const;
        Matrix4 GetInverseWorldMatrix() const;
        void RotateAround(const Vector3& vector, float degrees);
        void RotateTo(float angleAroundX, float angleAroundY, float angleAroundZ);
        void RotateBy(float deltaAngleAroundX, float deltaAngleAroundY, float deltaAngleAroundZ);
        void ScaleTo(float scaleX, float scaleY, float scaleZ);
        void ScaleBy(float scalarX, float scalarY, float scalarZ);
        void TranslateTo(float x, float y, float z);
        void TranslateBy(float deltaX, float deltaY, float deltaZ);
        void TranslateTo(const Vector3& position);
        void TranslateBy(const Vector3& delta);

    };
}
