#include <engine/core/solver.hpp>
#include <engine/core/physics.hpp>

#include <string>
#include <iostream>

namespace Engine
{
    bool Solver::GJK(const Collider& collider1, const Transform& transform1, const Collider& collider2, const Transform& transform2)
    {
        glm::vec3 initial = transform1.GetPosition() - transform2.GetPosition();
        glm::vec3 support = Minkowski(collider1, transform1, collider2, transform2, initial);

        Simplex simplex;
        simplex.push_front(support);

        glm::vec3 direction = -support;
        while (true)
        {
            support = Minkowski(collider1, transform1, collider2, transform2, direction);
    
            if (glm::dot(support, direction) <= 0) return false;

            simplex.push_front(support);
            if (NextSimplex(simplex, direction)) return true;
        }
    }
    glm::vec3 Solver::Minkowski(const Collider& collider1, const Transform& transform1, const Collider& collider2, const Transform& transform2, glm::vec3 direction)
    {
        return collider1.GetWorldSupport(transform1, direction) - collider2.GetWorldSupport(transform2, -direction);
    }
    bool Solver::SameDirection(const glm::vec3& direction, const glm::vec3& vector) { return glm::dot(direction, vector) > 0; }
    bool Solver::NextSimplex(Simplex& simplex, glm::vec3& direction)
    {
        switch (simplex.size())
        {
            case 2: return Line(simplex, direction);
            case 3: return Triangle(simplex, direction);
            case 4: return Tetrahedron(simplex, direction);
        }
        return false;
    }
    bool Solver::Line(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 A = simplex[0];
        glm::vec3 B = simplex[1];
        glm::vec3 AB = B - A;
        glm::vec3 AO = -A;
    
        if (SameDirection(AB, AO)) direction = glm::cross(glm::cross(AB, AO), AB);
        else
        {
            simplex = { A };
            direction = AO;
        }
        return false;
    }
    bool Solver::Triangle(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 A = simplex[0];
        glm::vec3 B = simplex[1];
        glm::vec3 C = simplex[2];

        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 AO = -A;
    
        glm::vec3 ABC = glm::cross(AB, AC);
    
        if (SameDirection(glm::cross(ABC, AC), AO))
        {
            if (SameDirection(AC, AO))
            {
                simplex = { A, C };
                direction = glm::cross(glm::cross(AC, AO), AC);
            }
            else return Line(simplex = { A, B }, direction);
        }
        else
        {
            if (SameDirection(glm::cross(AB, ABC), AO)) return Line(simplex = { A, B }, direction);
            else
            {
                if (SameDirection(ABC, AO)) direction = ABC;
                else
                {
                    simplex = { A, C, B };
                    direction = -ABC;
                }
            }
        }
        return false;
    }
    bool Solver::Tetrahedron(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 A = simplex[0];
        glm::vec3 B = simplex[1];
        glm::vec3 C = simplex[2];
        glm::vec3 D = simplex[3];

        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 AD = D - A;
        glm::vec3 AO = -A;
    
        glm::vec3 ABC = glm::cross(AB, AC);
        glm::vec3 ACD = glm::cross(AC, AD);
        glm::vec3 ADB = glm::cross(AD, AB);
    
        if (SameDirection(ABC, AO)) return Triangle(simplex = { A, B, C }, direction);
        if (SameDirection(ACD, AO)) return Triangle(simplex = { A, C, D }, direction);
        if (SameDirection(ADB, AO)) return Triangle(simplex = { A, D, B }, direction);
    
        return true;
    }
    void Solver::Solve(World& world, float deltaTime)
    {
        auto view = world.View<Transform, Physics>();
        for (auto [handle1, transform1, physics1] : view)
        {
            for (auto [handle2, transform2, physics2] : view)
            {
                if (handle1 == handle2) continue;
                if (GJK(physics1.GetCollider(), transform1, physics2.GetCollider(), transform2)) std::cout << std::format("{} and {} collided!\n", (int) handle1, (int) handle2);
            }
        }
    }
}