#include <engine/core/solver.hpp>
#include <engine/core/physics.hpp>

#include <string>
#include <iostream>
#include <random>

namespace Engine
{
    Solver::CollisionInfo Solver::GJK(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB)
    {
        CollisionInfo info;
        Simplex simplex = Simplex();

        glm::vec3 direction = transformA.GetPosition() - transformB.GetPosition();
        glm::vec3 support = GetSupport(colliderA, transformA, colliderB, transformB, direction);

        simplex.push_front(support);

        direction = -support;

        for (size_t iteration = 0; iteration < s_MaxGJKIterations; ++iteration)
        {
            support = GetSupport(colliderA, transformA, colliderB, transformB, direction);

            // Termination condition, Adding a check to prevent cyclic support points might be good here.
            if (!SameDirection(support, direction))
            {
                info.status = CollisionInfo::Status::Separated;
                return info;
            }

            simplex.push_front(support);

            if (NextSimplex(simplex, direction))
            {
                EPA(simplex, colliderA, transformA, colliderB, transformB, info);
                return info;
            }
        }

        info.status = CollisionInfo::Status::GJKFailed;
        return info;
    }
    bool Solver::SameDirection(const glm::vec3& vector, const glm::vec3& direction) { return glm::dot(vector, direction) > 0; }
    bool Solver::NextSimplex(Simplex& simplex, glm::vec3& direction)
    {
        switch (simplex.size())
        {
            case 2: return Line(simplex, direction);
            case 3: return Triangle(simplex, direction);
            case 4: return Tetrahedron(simplex, direction);
            default: return false;
        }
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
    
    void Solver::EPA(const Simplex& simplex, const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB, CollisionInfo& info)
    {
        std::vector<glm::vec3> polytope(simplex.begin(), simplex.end());
        std::vector<size_t> faces = {
            0, 1, 2,
            0, 3, 1,
            0, 2, 3,
            1, 3, 2
        };

        auto [normals, minFace] = GetFaceNormals(polytope, faces);

        glm::vec3  minNormal;
        float minDistance = std::numeric_limits<float>::max();
        constexpr const float epsilon = 0.01f;
        
        size_t iteration = 0;
        while (++iteration <= s_MaxEPAIterations && minDistance == std::numeric_limits<float>::max())
        {
            minNormal = glm::vec3(normals[minFace]);
            minDistance = normals[minFace].w;
    
            glm::vec3 support = GetSupport(colliderA, transformA, colliderB, transformB, minNormal);
            float sDistance = glm::dot(minNormal, support);
    
            if (std::abs(sDistance - minDistance) > epsilon)
            {
                minDistance = std::numeric_limits<float>::max();
                std::vector<std::pair<size_t, size_t>> uniqueEdges;

                for (size_t i = 0; i < normals.size(); i++)
                {
                    if (SameDirection(normals[i], support))
                    {
                        size_t f = i * 3;

                        AddIfUniqueEdge(uniqueEdges, faces, f,     f + 1);
                        AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                        AddIfUniqueEdge(uniqueEdges, faces, f + 2, f    );

                        faces[f + 2] = faces.back(); faces.pop_back();
                        faces[f + 1] = faces.back(); faces.pop_back();
                        faces[f    ] = faces.back(); faces.pop_back();

                        normals[i] = normals.back();
                        normals.pop_back();

                        i--;
                    }
                }

                std::vector<size_t> newFaces;
                for (auto [edgeIndex1, edgeIndex2] : uniqueEdges)
                {
                    newFaces.push_back(edgeIndex1);
                    newFaces.push_back(edgeIndex2);
                    newFaces.push_back(polytope.size());
                }

                if (newFaces.empty())
                {
                    info.status = CollisionInfo::Status::EPAFailed;
                    return;
                }
                
                polytope.push_back(support);

                auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);

                float oldMinDistance = std::numeric_limits<float>::max();
                for (size_t i = 0; i < normals.size(); i++)
                {
                    if (normals[i].w < oldMinDistance)
                    {
                        oldMinDistance = normals[i].w;
                        minFace = i;
                    }
                }
    
                if (newNormals[newMinFace].w < oldMinDistance) minFace = newMinFace + normals.size();
    
                faces.insert(faces.end(), newFaces.begin(), newFaces.end());
                normals.insert(normals.end(), newNormals.begin(), newNormals.end());
		    }
            else
            {
                info.depth = minDistance + epsilon;
                info.normal = minNormal;
                info.status = CollisionInfo::Status::Colliding;
                return;
            }
	    }

        info.status = CollisionInfo::Status::EPAFailed;
    }
    std::pair<std::vector<glm::vec4>, size_t> Solver::GetFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<size_t>& faces)
    {
        std::vector<glm::vec4> normals;
        float min = std::numeric_limits<float>::max();
        size_t index = 0;

        glm::vec3 A;
        glm::vec3 B;
        glm::vec3 C;

        for (size_t i = 0; i < faces.size(); i += 3)
        {
            A = polytope[faces[i]];
            B = polytope[faces[i + 1]];
            C = polytope[faces[i + 2]];

            glm::vec3 normal = glm::normalize(glm::cross(B - A, C - A));
            float distance = glm::dot(normal, A);

            if (distance < 0) 
            {
                normal *= -1;
                distance *= -1;
            }

            normals.emplace_back(normal, distance);

            if (distance < min)
            {
                min = distance;
                index = i / 3;
            }
        }

        return { normals, index };
    }
    void Solver::AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
    {
        auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));
        if (reverse != edges.end()) edges.erase(reverse);
        else edges.emplace_back(faces[a], faces[b]);
    }

    void Solver::Solve(World& world, float deltaTime)
    {
        auto view = world.View<Transform, Physics>();
        for (auto [handle, transform, physics] : view)
        {
            if (!physics.m_Stationary)
            {
                transform.RotateBy(deltaTime, deltaTime, deltaTime);
                physics.m_Force = glm::vec3(0);
                physics.m_Force += glm::vec3(0, 0, -1) * m_Gravity * physics.m_Mass;
                
                physics.m_Velocity += (physics.m_Force / physics.m_Mass) * deltaTime;
                physics.m_Velocity *= (1.0f - physics.m_Drag * deltaTime);

                transform.TranslateBy(physics.m_Velocity * deltaTime);
            }
        }
        for (auto [handleA, transformA, physicsA] : view)
        {
            for (auto [handleB, transformB, physicsB] : view)
            {
                if (handleA == handleB) continue;
                if (physicsA.IsStationary() && physicsB.IsStationary()) continue; 
                CollisionInfo collision = GJK(physicsA.GetCollider(), transformA, physicsB.GetCollider(), transformB);
                if (collision)
                {                    
                    glm::vec3 relativeVelocity = physicsB.m_Velocity - physicsA.m_Velocity;
                    float velocityAlongNormal = glm::dot(relativeVelocity, collision.normal);
                    glm::vec3 impulse;

                    if (velocityAlongNormal > 0) impulse = glm::vec3(0);
                    else
                    {
                        float e = std::min(physicsA.m_Restitution, physicsB.m_Restitution);
                        float j = -velocityAlongNormal * (1 + e);
                        j /= (1 / physicsA.m_Mass + 1 / physicsB.m_Mass);
                        impulse = j * collision.normal;
                    }

                    glm::vec3 seperator = collision.normal * collision.depth;
                    if (!physicsA.m_Stationary)
                    {
                        physicsA.m_Velocity -= impulse / physicsA.m_Mass;
                        transformA.TranslateBy(-seperator);
                    }
                    else if (!physicsB.m_Stationary)
                    {
                        physicsB.m_Velocity += impulse / physicsB.m_Mass;
                        transformB.TranslateBy(seperator);
                    }
                    else
                    {
                        physicsA.m_Velocity -= impulse / physicsA.m_Mass;
                        transformA.TranslateBy(-seperator * 0.5f);
                        physicsB.m_Velocity += impulse / physicsB.m_Mass;
                        transformB.TranslateBy(seperator * 0.5f);
                    }
                }
                else
                {
                    if (collision.status == CollisionInfo::Status::GJKFailed) std::cout << std::format("GJK failed between object {} and {}\n", (int) handleA, (int) handleB);
                    else if (collision.status == CollisionInfo::Status::EPAFailed) std::cout << std::format("EPA failed between object {} and {}\n", (int) handleA, (int) handleB);
                }
            }
        }
    }
}