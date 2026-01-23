#pragma once
#include <engine/core/math.hpp>
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
            Vector3 point;
            Vector3 pointFromA;
            Vector3 pointFromB;
        };

        struct Simplex
        {
            size_t count = 0;
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
            Vector3 normal;
            float distance;

            Face(size_t a, size_t b, size_t c, const std::vector<Support>& polytope) : a(a), b(b), c(c)
            {
                normal = Normalized(Cross(polytope[b].point - polytope[a].point, polytope[c].point - polytope[a].point));
                distance = Dot(normal, polytope[a].point);
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
            Vector3 contactPointA; // Furthest point of A into B.
            Vector3 contactPointB; // Furthest point of B into A.
            Vector3 normal;
            float depth;

            operator bool() { return status == Status::Colliding; }
        };

        float m_Gravity = 9.81f;

        static constexpr size_t s_MaxGJKIterations = 32;
        static constexpr size_t s_MaxEPAIterations = 64;

        Support GetSupport(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB, Vector3 direction);
        inline Vector3 ConvertToBarycentric(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c);
        inline bool SameDirection(const Vector3& u, const Vector3& v);

        CollisionInfo GJK(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB);
        bool NextSimplex(Simplex& simplex, Vector3& direction);
        bool Line(Simplex& simplex, Vector3& direction);
        bool Triangle(Simplex& simplex, Vector3& direction);
        bool Tetrahedron(Simplex& simplex, Vector3& direction);
        CollisionInfo EPA(const Simplex& simplex, const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB);

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
