#pragma once
#include <engine/core/math.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Transform : public Component
    {
        public:

        Vector3 m_Position = Vector3(0.0f);
        Vector3 m_Scale = Vector3(1.0f);
        Quaternion m_Orientation = Quaternion(0.0f, 0.0f, 0.0f);

        public:

        Transform() = default;
        Transform(const Vector3& position);
        Transform(const Quaternion& orientation);
        Transform(const Vector3& position, const Vector3& scale);
        Transform(const Vector3& position, const Quaternion& orientation);
        Transform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);
        Transform(const Transform& other) = default;
        Transform(Transform&& other) = default;
        Transform& operator=(const Transform& other) = default;
        Transform& operator=(Transform&& other) = default;
        ~Transform() = default;

        Vector3 GetPosition() const;
        Vector3 GetInversePosition() const;
        Vector3 GetScale() const;
        Vector3 GetInverseScale() const;
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
        void TranslateTo(const Vector3& position);
        void TranslateBy(const Vector3& delta);
        void ScaleTo(const Vector3& scale);
        void ScaleBy(const Vector3& delta);
        void RotateTo(const Quaternion& orientation);
        void RotateBy(const Quaternion& delta);
        void RotateAround(const Vector3& axis, float radians);

    };
}
