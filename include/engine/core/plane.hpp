#pragma once
#include <glm/glm.hpp>

namespace Engine
{
    class Plane
    {
        private:
        
        float m_Distance = 0.0f;
        glm::vec3 m_Normal = glm::vec3(0.0f, 0.0f, 1.0f);

        public:
    
        constexpr Plane() = default;
        constexpr Plane(const glm::vec3& direction, float distance) : m_Normal(glm::normalize(direction)), m_Distance(distance) {}
        constexpr Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
        {
            m_Normal = glm::normalize(glm::cross(b - a, c - a));
            m_Distance = glm::dot(m_Normal, a);
        }
        ~Plane() = default;
        
        inline float GetDistance() const { return m_Distance; }
        inline const glm::vec3& GetNormal() const { return m_Normal; }
        inline glm::vec3 GetProjection(const glm::vec3& point) const { return point - GetSignedDistance(point) * m_Normal; }
        inline float GetSignedDistance(const glm::vec3& point) const { return glm::dot(m_Normal, point) - m_Distance; }
        inline bool IsContained(const glm::vec3& point, float epsilon = 1e-3f) const { return std::abs(GetSignedDistance(point)) < std::abs(epsilon); }
        inline bool IsInfront(const glm::vec3& point, float epsilon = 0.0f) const { return GetSignedDistance(point) > std::abs(epsilon); }
        inline bool IsBehind(const glm::vec3& point, float epsilon = 0.0f) const { return GetSignedDistance(point) < -std::abs(epsilon); }

    };
}