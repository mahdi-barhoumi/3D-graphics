#pragma once
#include <engine/core/math.hpp>

namespace Engine
{
    class Plane
    {
        private:
        
        float m_Distance = 0.0f;
        Vector3 m_Normal = Vector3(0.0f, 0.0f, 1.0f);

        public:
    
        constexpr Plane() = default;
        constexpr Plane(const Vector3& direction, float distance) : m_Normal(Normalized(direction)), m_Distance(distance) {}
        constexpr Plane(const Vector3& a, const Vector3& b, const Vector3& c)
        {
            m_Normal = Normalized(Cross(b - a, c - a));
            m_Distance = Dot(m_Normal, a);
        }
        ~Plane() = default;
        
        inline float GetDistance() const { return m_Distance; }
        inline const Vector3& GetNormal() const { return m_Normal; }
        inline Vector3 GetProjection(const Vector3& point) const { return point - GetSignedDistance(point) * m_Normal; }
        inline float GetSignedDistance(const Vector3& point) const { return Dot(m_Normal, point) - m_Distance; }
        inline bool IsContained(const Vector3& point, float epsilon = 1e-3f) const { return Abs(GetSignedDistance(point)) < Abs(epsilon); }
        inline bool IsInfront(const Vector3& point, float epsilon = 0.0f) const { return GetSignedDistance(point) > Abs(epsilon); }
        inline bool IsBehind(const Vector3& point, float epsilon = 0.0f) const { return GetSignedDistance(point) < -Abs(epsilon); }
        
    };
}