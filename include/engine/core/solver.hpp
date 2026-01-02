#pragma once
#include <glm/glm.hpp>
#include <engine/core/world.hpp>
#include <engine/core/physics.hpp>
#include <engine/core/collider.hpp>
#include <engine/core/transform.hpp>

namespace Engine
{
    class Solver
    {
        private:

        struct Support
        {
            glm::vec3 point;
            glm::vec3 pointFromA;
            glm::vec3 pointFromB;
        };

        struct Simplex
        {
            size_t count;
            Support A;
            Support B;
            Support C;
            Support D;

            void Push(Support support)
            {
                D = C;
                C = B;
                B = A;
                A = support;
                count = std::min(count + 1, static_cast<size_t>(4));
            }

        };

        struct Face
        {
            size_t a;
            size_t b;
            size_t c;
            glm::vec3 normal;
            float distance;

            Face(size_t a, size_t b, size_t c, const std::vector<Support>& polytope) : a(a), b(b), c(c)
            {
                normal = glm::normalize(glm::cross(polytope[b].point - polytope[a].point, polytope[c].point - polytope[a].point));
                distance = glm::dot(normal, polytope[a].point);
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
            glm::vec3 contactPointA; // Furthest point of A into B.
            glm::vec3 contactPointB; // Furthest point of B into A.
            glm::vec3 normal;
            float depth;

            operator bool() { return status == Status::Colliding; }
        };

        float m_Gravity = 9.81f;

        static constexpr size_t s_MaxGJKIterations = 32;
        static constexpr size_t s_MaxEPAIterations = 64;

        Support GetSupport(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB, glm::vec3 direction);

        CollisionInfo GJK(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB);
        bool NextSimplex(Simplex& simplex, glm::vec3& direction);
        bool Line(Simplex& simplex, glm::vec3& direction);
        bool Triangle(Simplex& simplex, glm::vec3& direction);
        bool Tetrahedron(Simplex& simplex, glm::vec3& direction);

        CollisionInfo EPA(const Simplex& simplex, const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB);
        void AddUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, size_t a, size_t b);

        inline bool SameDirection(const glm::vec3& u, const glm::vec3& v);
        inline glm::vec3 ConvertToBarycentric(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

        void ResolveCollision(Physics& physicsA, Transform& transformA, Physics& physicsB, Transform& transformB, const CollisionInfo& collision);

        public:

        Solver() = default;
        Solver(float gravity);
        ~Solver() = default;
        float GetGravity() const;
        void SetGravity(float gravity);
        void Solve(World& world, float deltaTime);

    };
}
