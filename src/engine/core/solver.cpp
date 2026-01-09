#include <engine/core/solver.hpp>

#include <string>
#include <iostream>
#include <random>

namespace Engine
{
    Solver::Solver(float gravity) : m_Gravity(gravity) {}
    float Solver::GetGravity() const { return m_Gravity; }
    void Solver::SetGravity(float gravity) { m_Gravity = gravity; }

    Solver::Support Solver::GetSupport(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB, glm::vec3 direction)
    {
        Support support;
        support.pointFromA = colliderA.GetWorldSupport(transformA, direction);
        support.pointFromB = colliderB.GetWorldSupport(transformB, -direction);
        support.point = support.pointFromA - support.pointFromB;
        return support;
    }
    
    bool Solver::SameDirection(const glm::vec3& u, const glm::vec3& v) { return glm::dot(u, v) > 0.0f; }
    glm::vec3 Solver::ConvertToBarycentric(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
    {
        glm::vec3 V0 = b - a;
        glm::vec3 V1 = c - a;
        glm::vec3 V2 = point - a;
        
        float D00 = glm::dot(V0, V0);
        float D01 = glm::dot(V0, V1);
        float D11 = glm::dot(V1, V1);
        float D20 = glm::dot(V2, V0);
        float D21 = glm::dot(V2, V1);
        
        float denominator = D00 * D11 - D01 * D01;
        
        // Degenerate triangle, return equal weights
        if (std::abs(denominator) < 1e-10f) return glm::vec3(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f);
        
        float v = (D11 * D20 - D01 * D21) / denominator;
        float w = (D00 * D21 - D01 * D20) / denominator;
        float u = 1.0f - v - w;
        
        return glm::vec3(u, v, w);
    }

    Solver::CollisionInfo Solver::GJK(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB)
    {
        Simplex simplex = Simplex();

        glm::vec3 direction = transformA.GetPosition() - transformB.GetPosition() + glm::vec3(1e-6f);
        Support support = GetSupport(colliderA, transformA, colliderB, transformB, direction);

        simplex.Push(support);
        direction = -support.point;

        for (size_t iteration = 0; iteration < s_MaxGJKIterations; ++iteration)
        {
            support = GetSupport(colliderA, transformA, colliderB, transformB, direction);

            if (!SameDirection(support.point, direction))
            {
                CollisionInfo info;
                info.status = CollisionInfo::Status::Separated;
                return info;
            }

            simplex.Push(support);

            if (NextSimplex(simplex, direction)) return EPA(simplex, colliderA, transformA, colliderB, transformB);
        }

        CollisionInfo info;
        info.status = CollisionInfo::Status::GJKFailed;
        return info;
    }
    bool Solver::NextSimplex(Simplex& simplex, glm::vec3& direction)
    {
        switch (simplex.count)
        {
            case 2: return Line(simplex, direction);
            case 3: return Triangle(simplex, direction);
            case 4: return Tetrahedron(simplex, direction);
            default: return false;
        }
    }
    bool Solver::Line(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 A = simplex.A.point;
        glm::vec3 B = simplex.B.point;
        glm::vec3 AB = B - A;
        glm::vec3 AO = -A;
    
        if (SameDirection(AB, AO)) direction = glm::cross(glm::cross(AB, AO), AB);
        else
        {
            simplex.count = 1;
            direction = AO;
        }
        return false;
    }
    bool Solver::Triangle(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 A = simplex.A.point;
        glm::vec3 B = simplex.B.point;
        glm::vec3 C = simplex.C.point;

        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 AO = -A;
    
        glm::vec3 ABC = glm::cross(AB, AC);
    
        if (SameDirection(glm::cross(ABC, AC), AO))
        {
            if (SameDirection(AC, AO))
            {
                simplex.count = 2;
                simplex.B = simplex.C;
                direction = glm::cross(glm::cross(AC, AO), AC);
            }
            else
            {
                simplex.count = 2;
                return Line(simplex, direction);
            }
        }
        else
        {
            if (SameDirection(glm::cross(AB, ABC), AO))
            {
                simplex.count = 2;
                return Line(simplex, direction);
            }
            else
            {
                if (SameDirection(ABC, AO)) direction = ABC;
                else
                {
                    std::swap(simplex.B, simplex.C);
                    direction = -ABC;
                }
            }
        }
        return false;
    }
    bool Solver::Tetrahedron(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 A = simplex.A.point;
        glm::vec3 B = simplex.B.point;
        glm::vec3 C = simplex.C.point;
        glm::vec3 D = simplex.D.point;

        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 AD = D - A;
        glm::vec3 AO = -A;
    
        glm::vec3 ABC = glm::cross(AB, AC);
        glm::vec3 ACD = glm::cross(AC, AD);
        glm::vec3 ADB = glm::cross(AD, AB);

        if (SameDirection(ABC, AO))
        {
            simplex.count = 3;
            return Triangle(simplex, direction);
        }
        if (SameDirection(ACD, AO))
        {
            simplex.count = 3;
            simplex.B = simplex.C;
            simplex.C = simplex.D;
            return Triangle(simplex, direction);
        }
        if (SameDirection(ADB, AO))
        {
            simplex.count = 3;
            simplex.C = simplex.B;
            simplex.B = simplex.D;
            return Triangle(simplex, direction);
        }

        return true;
    }
    
    Solver::CollisionInfo Solver::EPA(const Simplex& simplex, const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB)
    {
        std::vector<Support> polytope;
        polytope.reserve(64);
        std::vector<Face> faces;
        faces.reserve(64);
        std::vector<std::pair<size_t, size_t>> uniqueEdges;
        uniqueEdges.reserve(64);

        polytope.push_back(simplex.A);
        polytope.push_back(simplex.B);
        polytope.push_back(simplex.C);
        polytope.push_back(simplex.D);
        faces.push_back(Face(0, 1, 2, polytope));   // Add face ABC to the polytope.
        faces.push_back(Face(0, 3, 1, polytope));   // Add face ADB to the polytope.
        faces.push_back(Face(0, 2, 3, polytope));   // Add face ACD to the polytope.
        faces.push_back(Face(1, 3, 2, polytope));   // Add face BDC to the polytope.

        Support support;
        glm::vec3 direction;
        size_t closestFace;
        float minDistance;
        float distance;

        for (size_t iteration = 0; iteration < s_MaxEPAIterations; ++iteration)
        {
            // Find closest face to the origin.
            closestFace = 0;
            minDistance = faces[0].distance;
            for (size_t index = 1; index < faces.size(); ++index)
            {
                if (faces[index].distance < minDistance)
                {
                    minDistance = faces[index].distance;
                    closestFace = index;
                }
            }

            // Search towards the normal of the face that's closest to origin.
            direction = faces[closestFace].normal;
            support = GetSupport(colliderA, transformA, colliderB, transformB, direction);

            if (glm::dot(support.point, direction) - minDistance < 1e-3f)
            {
                CollisionInfo info;
                info.status = CollisionInfo::Status::Colliding;
                info.depth = minDistance;
                info.normal = faces[closestFace].normal;
                
                // Project origin onto the closest face plane
                glm::vec3 originProjection = minDistance * faces[closestFace].normal;
                
                glm::vec3 barycentricCoordinates = ConvertToBarycentric(originProjection,
                                                    polytope[faces[closestFace].a].point,
                                                    polytope[faces[closestFace].b].point,
                                                    polytope[faces[closestFace].c].point
                                                );
                
                info.contactPointA = polytope[faces[closestFace].a].pointFromA * barycentricCoordinates.x +
                                     polytope[faces[closestFace].b].pointFromA * barycentricCoordinates.y +
                                     polytope[faces[closestFace].c].pointFromA * barycentricCoordinates.z;
                
                info.contactPointB = polytope[faces[closestFace].a].pointFromB * barycentricCoordinates.x +
                                     polytope[faces[closestFace].b].pointFromB * barycentricCoordinates.y +
                                     polytope[faces[closestFace].c].pointFromB * barycentricCoordinates.z;
                
                return info;
            }

            uniqueEdges.clear();
            for (size_t index = 0; index < faces.size();)
            {
                if (SameDirection(faces[index].normal, support.point - polytope[faces[index].a].point))
                {
                    // Add unique edges only, if a reverse edge aleady exists then remove it (both triangles it belonged to are removed).
                    AddUniqueEdge(uniqueEdges, faces[index].a, faces[index].b);
                    AddUniqueEdge(uniqueEdges, faces[index].b, faces[index].c);
                    AddUniqueEdge(uniqueEdges, faces[index].c, faces[index].a);

                    // Remove the face.
                    faces[index] = faces.back();
                    faces.pop_back();
                }
                else ++index;
            }

            // We can now add the new support point into the polytope.
            polytope.push_back(support);
            for (auto [a, b] : uniqueEdges) faces.push_back(Face(a, b, polytope.size() - 1, polytope));
        }

        CollisionInfo info;
        info.status = CollisionInfo::Status::EPAFailed;
        return info;
    }
    void Solver::AddUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, size_t a, size_t b)
    {
        auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(b, a));
        if (reverse != edges.end()) edges.erase(reverse);
        else edges.emplace_back(a, b);
    }

    void Solver::ResolveCollision(Physics& physicsA, Transform& transformA, Physics& physicsB, Transform& transformB, const CollisionInfo& collision)
    {
        // Calculate relative positions from center of mass to contact point.
        glm::vec3 relativeA = collision.contactPointA - transformA.GetPosition();
        glm::vec3 relativeB = collision.contactPointB - transformB.GetPosition();
        
        // Calculate velocity at contact point (linear + angular contribution).
        glm::vec3 angularVelocityA = glm::cross(physicsA.GetAngularVelocity(), relativeA);
        glm::vec3 angularVelocityB = glm::cross(physicsB.GetAngularVelocity(), relativeB);
        
        glm::vec3 fullVelocityA = physicsA.GetVelocity() + angularVelocityA;
        glm::vec3 fullVelocityB = physicsB.GetVelocity() + angularVelocityB;
        
        glm::vec3 relativeVelocity = fullVelocityB - fullVelocityA;
        float velocityAlongNormal = glm::dot(relativeVelocity, collision.normal);
        
        // Calculate angular effect on impulse (only for non-stationary objects).
        glm::vec3 inertiaA = glm::vec3(0);
        glm::vec3 inertiaB = glm::vec3(0);
        glm::mat3 inverseInertiaTensorWorldA = glm::mat3(0);
        glm::mat3 inverseInertiaTensorWorldB = glm::mat3(0);
        
        // TODO: This is wrong and bad (incorrect inertia scaling calculations).
        if (!physicsA.IsStationary())
        {
            glm::mat3 rotationMatrixA = glm::mat3(transformA.GetRotationMatrix());
            glm::mat3 inverseScalingMatrixA = glm::mat3(transformA.GetInverseScalingMatrix());
            inverseInertiaTensorWorldA = rotationMatrixA * inverseScalingMatrixA * physicsA.GetInverseInertiaTensor() * inverseScalingMatrixA * glm::transpose(rotationMatrixA);
            inertiaA = glm::cross(inverseInertiaTensorWorldA * glm::cross(relativeA, collision.normal), relativeA);
        }
        if (!physicsB.IsStationary())
        {
            glm::mat3 rotationMatrixB = glm::mat3(transformB.GetRotationMatrix());
            glm::mat3 inverseScalingMatrixB = glm::mat3(transformB.GetInverseScalingMatrix());
            inverseInertiaTensorWorldB = rotationMatrixB * inverseScalingMatrixB * physicsB.GetInverseInertiaTensor() * inverseScalingMatrixB * glm::transpose(rotationMatrixB);
            inertiaB = glm::cross(inverseInertiaTensorWorldB * glm::cross(relativeB, collision.normal), relativeB);
        }
        float angularEffect = glm::dot(inertiaA + inertiaB, collision.normal);
        
        // Calculate normal impulse magnitude.
        float e = std::min(physicsA.GetRestitution(), physicsB.GetRestitution());
        float j = -(1.0f + e) * velocityAlongNormal;
        j /= (physicsA.GetInverseMass() + physicsB.GetInverseMass() + angularEffect);
        
        glm::vec3 impulse = j * collision.normal;
        
        // Apply linear impulses.
        if (!physicsA.IsStationary()) physicsA.ApplyLinearImpulse(-impulse);
        if (!physicsB.IsStationary()) physicsB.ApplyLinearImpulse(impulse);
        
        // Apply angular impulses.
        if (!physicsA.IsStationary()) physicsA.ApplyAngularImpulse(glm::cross(relativeA, -impulse), inverseInertiaTensorWorldA);
        if (!physicsB.IsStationary()) physicsB.ApplyAngularImpulse(glm::cross(relativeB, impulse), inverseInertiaTensorWorldB);
        
        // Calculate and apply friction impulses.
        glm::vec3 tangent = relativeVelocity - velocityAlongNormal * collision.normal;
        float tangentLength = glm::length(tangent);
        
        if (tangentLength > 0.0001f) // Avoid division by zero
        {
            tangent /= tangentLength; // Normalize tangent
            
            // Calculate angular effect for tangent direction.
            glm::vec3 inertiaTangentA = glm::vec3(0);
            glm::vec3 inertiaTangentB = glm::vec3(0);
            
            if (!physicsA.IsStationary())
            {
                inertiaTangentA = glm::cross(inverseInertiaTensorWorldA * glm::cross(relativeA, tangent), relativeA);
            }
            if (!physicsB.IsStationary())
            {
                inertiaTangentB = glm::cross(inverseInertiaTensorWorldB * glm::cross(relativeB, tangent), relativeB);
            }
            float angularEffectTangent = glm::dot(inertiaTangentA + inertiaTangentB, tangent);
            
            // Calculate friction impulse magnitude.
            float jt = -glm::dot(relativeVelocity, tangent);
            jt /= (physicsA.GetInverseMass() + physicsB.GetInverseMass() + angularEffectTangent);
            
            // Apply Coulomb friction (static vs dynamic).
            float mu = (physicsA.GetFriction() + physicsB.GetFriction()) * 0.5f;
            glm::vec3 frictionImpulse;
            
            if (std::abs(jt) < j * mu)
            {
                // Static friction
                frictionImpulse = jt * tangent;
            }
            else
            {
                // Dynamic friction
                frictionImpulse = -j * mu * tangent;
            }
            
            // Apply friction impulses.
            if (!physicsA.IsStationary()) physicsA.ApplyLinearImpulse(-frictionImpulse);
            if (!physicsB.IsStationary()) physicsB.ApplyLinearImpulse(frictionImpulse);
            
            // Apply angular friction impulses.
            if (!physicsA.IsStationary()) physicsA.ApplyAngularImpulse(glm::cross(relativeA, -frictionImpulse), inverseInertiaTensorWorldA);
            if (!physicsB.IsStationary()) physicsB.ApplyAngularImpulse(glm::cross(relativeB, frictionImpulse), inverseInertiaTensorWorldB);
        }
        
        // Separate objects.
        float totalInverseMass = physicsA.GetInverseMass() + physicsB.GetInverseMass();
        if (totalInverseMass > 0)
        {
            if (!physicsA.IsStationary()) transformA.TranslateBy(-collision.normal * collision.depth * (physicsA.GetInverseMass() / totalInverseMass));
            if (!physicsB.IsStationary()) transformB.TranslateBy(collision.normal * collision.depth * (physicsB.GetInverseMass() / totalInverseMass));
        }
    }

    void Solver::Solve(World& world, float deltaTime)
    {
        deltaTime = std::clamp(deltaTime, 0.0f, 1.0f);
        auto view = world.View<Transform, Physics>();
        for (auto [handle, transform, physics] : view)
        {
            if (physics.IsStationary()) continue;

            physics.ApplyForce(m_Gravity * physics.GetMass() * glm::vec3(0, 0, -1));
            
            glm::mat3 rotationMatrix = glm::mat3(transform.GetRotationMatrix());
            glm::mat3 inverseScalingMatrix = glm::mat3(transform.GetInverseScalingMatrix());
            glm::mat3 inverseInertiaTensorWorld = rotationMatrix * inverseScalingMatrix * physics.GetInverseInertiaTensor() * inverseScalingMatrix * glm::transpose(rotationMatrix);
            physics.Integrate(deltaTime, inverseInertiaTensorWorld);

            transform.TranslateBy(physics.GetVelocity() * deltaTime);
            
            glm::vec3 angularVelocity = physics.GetAngularVelocity();
            float deltaAngle = glm::length(angularVelocity) * deltaTime;
            if (deltaAngle > 1e-6f)
            {
                glm::vec3 rotationAxis = glm::normalize(angularVelocity);
                transform.RotateAround(rotationAxis, deltaAngle);
            }
            
            physics.ResetAccumulators();
        }
        for (auto [handleA, transformA, physicsA] : view)
        {
            for (auto [handleB, transformB, physicsB] : view)
            {
                if (handleA <= handleB) continue;
                if (physicsA.IsStationary() && physicsB.IsStationary()) continue; 
                CollisionInfo collision = GJK(physicsA.GetCollider(), transformA, physicsB.GetCollider(), transformB);
                if (collision) ResolveCollision(physicsA, transformA, physicsB, transformB, collision);
                else
                {
                    if (collision.status == CollisionInfo::Status::GJKFailed) std::cout << std::format("GJK failed between object {} and {}\n", (int) handleA, (int) handleB);
                    else if (collision.status == CollisionInfo::Status::EPAFailed) std::cout << std::format("EPA failed between object {} and {}\n", (int) handleA, (int) handleB);
                }
            }
        }
    }
}