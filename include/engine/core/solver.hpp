#pragma once
#include <array>
#include <initializer_list>
#include <glm/glm.hpp>
#include <engine/core/world.hpp>
#include <engine/core/collider.hpp>
#include <engine/core/transform.hpp>

namespace Engine
{
    class Solver
    {
        private:

        class Simplex
        {
            private:

            unsigned char m_Size = 0;
            std::array<glm::vec3, 4> m_Points;

            public:

            Simplex& operator=(std::initializer_list<glm::vec3> list)
            {
                m_Size = 0;
                for (glm::vec3 point : list) m_Points[m_Size++] = point;
                return *this;
            }
            glm::vec3& operator[](int i) { return m_Points[i]; }

            auto end() const { return m_Points.end() - (4 - m_Size); }
            auto begin() const { return m_Points.begin(); }
            size_t size() const { return m_Size; }
            void push_front(glm::vec3 point)
            {
                m_Points = { point, m_Points[0], m_Points[1], m_Points[2] };
                m_Size = std::min(m_Size + 1, 4);
            }

        };

        struct CollisionInfo
        {
            enum class Status
            {
                Separated,
                Colliding,
                GJKFailed,
                EPAFailed,
            };

            Status status;
            glm::vec3 A;
            glm::vec3 B;
            glm::vec3 contactPoint;
            glm::vec3 normal;
            float depth;

            operator bool() { return status == Status::Colliding; }
        };

        float m_Gravity = 9.81f;

        static constexpr size_t s_MaxGJKIterations = 32;
        static constexpr size_t s_MaxEPAIterations = 64;

        CollisionInfo GJK(const Collider& collider1, const Transform& transform1, const Collider& collider2, const Transform& transform2);
        glm::vec3 Minkowski(const Collider& collider1, const Transform& transform1, const Collider& collider2, const Transform& transform2, glm::vec3 direction);
        bool NextSimplex(Simplex& simplex, glm::vec3& direction);
        bool Line(Simplex& simplex, glm::vec3& direction);
        bool Triangle(Simplex& simplex, glm::vec3& direction);
        bool Tetrahedron(Simplex& simplex, glm::vec3& direction);
        inline bool SameDirection(const glm::vec3& direction, const glm::vec3& vector);

        void EPA(const Simplex& simplex, const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB, CollisionInfo& info);
        std::pair<std::vector<glm::vec4>, size_t> GetFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<size_t>& faces);
        void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

        public:

        void Solve(World& world, float deltaTime);

    };
}
