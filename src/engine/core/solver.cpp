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

        m_Simplex = Simplex();
        m_Simplex.push_front(support);

        glm::vec3 direction = -support;
        while (true)
        {
            support = Minkowski(collider1, transform1, collider2, transform2, direction);
    
            if (glm::dot(support, direction) <= 0) return false;

            m_Simplex.push_front(support);
            if (NextSimplex(m_Simplex, direction)) return true;
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
    
    Solver::Collision Solver::EPA(const Simplex& simplex, const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB)
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
        float minDistance = FLT_MAX;
        
        while (minDistance == FLT_MAX)
        {
            minNormal   = glm::vec3(normals[minFace]);
            minDistance = normals[minFace].w;
    
            glm::vec3 support = Minkowski(colliderA, transformA, colliderB, transformB, minNormal);
            float sDistance = glm::dot(minNormal, support);
    
            if (abs(sDistance - minDistance) > 0.001f)
            {
                minDistance = FLT_MAX;
                std::vector<std::pair<size_t, size_t>> uniqueEdges;

                for (size_t i = 0; i < normals.size(); i++)
                {
                    if (SameDirection(normals[i], support)) {
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
                
                polytope.push_back(support);

                auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);
                float oldMinDistance = FLT_MAX;
                for (size_t i = 0; i < normals.size(); i++)
                {
                    if (normals[i].w < oldMinDistance)
                    {
                        oldMinDistance = normals[i].w;
                        minFace = i;
                    }
                }
    
                if (newNormals[newMinFace].w < oldMinDistance) minFace = newMinFace + normals.size();
    
                faces  .insert(faces  .end(), newFaces  .begin(), newFaces  .end());
                normals.insert(normals.end(), newNormals.begin(), newNormals.end());
		    }
	    }
 
        Collision collision;
 
        collision.normal = minNormal;
        collision.depth = minDistance + 0.001f;
        collision.colliding = true;
    
        return collision;
    }
    std::pair<std::vector<glm::vec4>, size_t> Solver::GetFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<size_t>& faces)
    {
        std::vector<glm::vec4> normals;
        size_t minTriangle = 0;
        float  minDistance = FLT_MAX;

        for (size_t i = 0; i < faces.size(); i += 3) {
            glm::vec3 a = polytope[faces[i    ]];
            glm::vec3 b = polytope[faces[i + 1]];
            glm::vec3 c = polytope[faces[i + 2]];

            glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
            float distance = glm::dot(normal, a);

            if (distance < 0) 
            {
                normal   *= -1;
                distance *= -1;
            }

            normals.emplace_back(normal, distance);

            if (distance < minDistance)
            {
                minTriangle = i / 3;
                minDistance = distance;
            }
        }

        return { normals, minTriangle };
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
        for (auto [handle1, transform1, physics1] : view)
        {
            for (auto [handle2, transform2, physics2] : view)
            {
                if (handle1 == handle2) continue;
                if (GJK(physics1.GetCollider(), transform1, physics2.GetCollider(), transform2))
                {
                    std::cout << std::format("{} and {} collided!\n", (int) handle1, (int) handle2);
                    Collision collision = EPA(m_Simplex, physics1.GetCollider(), transform1, physics2.GetCollider(), transform2);
                    glm::vec3 seperator = collision.normal * collision.depth * 0.5f;
                    transform2.TranslateBy(seperator.x, seperator.y, seperator.z);
                    transform1.TranslateBy(-seperator.x, -seperator.y, -seperator.z);
                }
            }
        }
    }
}