#include <set>
#include <vector>
#include <engine/core/solver.hpp>

// #include <string>
// #include <iostream>

namespace Engine
{
    Solver::Solver(float gravity) : m_Gravity(gravity) {}
    float Solver::GetGravity() const { return m_Gravity; }
    void Solver::SetGravity(float gravity) { m_Gravity = gravity; }

    Solver::Support Solver::GetSupport(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB, Vector3 direction)
    {
        Support support;
        support.pointFromA = colliderA.GetWorldSupport(transformA, direction);
        support.pointFromB = colliderB.GetWorldSupport(transformB, -direction);
        support.point = support.pointFromA - support.pointFromB;
        return support;
    }
    Vector3 Solver::ConvertToBarycentric(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c)
    {
        Vector3 V0 = b - a;
        Vector3 V1 = c - a;
        Vector3 V2 = point - a;
        
        float D00 = Dot(V0, V0);
        float D01 = Dot(V0, V1);
        float D11 = Dot(V1, V1);
        float D20 = Dot(V2, V0);
        float D21 = Dot(V2, V1);
        
        float denominator = D00 * D11 - D01 * D01;
        
        // Degenerate triangle, return equal weights
        if (Abs(denominator) < 1e-10f) return Vector3(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f);
        
        float v = (D11 * D20 - D01 * D21) / denominator;
        float w = (D00 * D21 - D01 * D20) / denominator;
        float u = 1.0f - v - w;
        
        return Vector3(u, v, w);
    }
    bool Solver::SameDirection(const Vector3& u, const Vector3& v) { return Dot(u, v) > 0.0f; }

    Solver::CollisionInfo Solver::GJK(const Collider& colliderA, const Transform& transformA, const Collider& colliderB, const Transform& transformB)
    {
        Simplex simplex;

        Vector3 direction = transformA.GetPosition() - transformB.GetPosition() + Vector3(1e-6f);
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
    bool Solver::NextSimplex(Simplex& simplex, Vector3& direction)
    {
        switch (simplex.count)
        {
            case 2: return Line(simplex, direction);
            case 3: return Triangle(simplex, direction);
            case 4: return Tetrahedron(simplex, direction);
            default: return false;
        }
    }
    bool Solver::Line(Simplex& simplex, Vector3& direction)
    {
        Vector3 A = simplex.A.point;
        Vector3 B = simplex.B.point;
        Vector3 AB = B - A;
        Vector3 AO = -A;
    
        if (SameDirection(AB, AO)) direction = Cross(Cross(AB, AO), AB);
        else
        {
            simplex.count = 1;
            direction = AO;
        }
        return false;
    }
    bool Solver::Triangle(Simplex& simplex, Vector3& direction)
    {
        Vector3 A = simplex.A.point;
        Vector3 B = simplex.B.point;
        Vector3 C = simplex.C.point;

        Vector3 AB = B - A;
        Vector3 AC = C - A;
        Vector3 AO = -A;
    
        Vector3 ABC = Cross(AB, AC);
    
        if (SameDirection(Cross(ABC, AC), AO))
        {
            if (SameDirection(AC, AO))
            {
                simplex.count = 2;
                simplex.B = simplex.C;
                direction = Cross(Cross(AC, AO), AC);
            }
            else
            {
                simplex.count = 2;
                return Line(simplex, direction);
            }
        }
        else
        {
            if (SameDirection(Cross(AB, ABC), AO))
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
    bool Solver::Tetrahedron(Simplex& simplex, Vector3& direction)
    {
        Vector3 A = simplex.A.point;
        Vector3 B = simplex.B.point;
        Vector3 C = simplex.C.point;
        Vector3 D = simplex.D.point;

        Vector3 AB = B - A;
        Vector3 AC = C - A;
        Vector3 AD = D - A;
        Vector3 AO = -A;
    
        Vector3 ABC = Cross(AB, AC);
        Vector3 ACD = Cross(AC, AD);
        Vector3 ADB = Cross(AD, AB);

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
        std::set<std::pair<size_t, size_t>> uniqueEdges;

        polytope.push_back(simplex.A);
        polytope.push_back(simplex.B);
        polytope.push_back(simplex.C);
        polytope.push_back(simplex.D);
        faces.push_back(Face(0, 1, 2, polytope));   // Add face ABC to the polytope.
        faces.push_back(Face(0, 3, 1, polytope));   // Add face ADB to the polytope.
        faces.push_back(Face(0, 2, 3, polytope));   // Add face ACD to the polytope.
        faces.push_back(Face(1, 3, 2, polytope));   // Add face BDC to the polytope.

        Support support;
        Vector3 direction;
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

            if (Dot(support.point, direction) - minDistance < 1e-3f)
            {
                CollisionInfo info;
                info.status = CollisionInfo::Status::Colliding;
                info.depth = minDistance;
                info.normal = faces[closestFace].normal;
                
                // Project origin onto the closest face plane
                Vector3 originProjection = minDistance * faces[closestFace].normal;
                
                Vector3 barycentricCoordinates = ConvertToBarycentric(originProjection,
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
                    size_t a = faces[index].a;
                    size_t b = faces[index].b;
                    size_t c = faces[index].c;

                    auto reverse = std::find(uniqueEdges.begin(), uniqueEdges.end(), std::make_pair(b, a));
                    if (reverse != uniqueEdges.end()) uniqueEdges.erase(reverse);
                    else uniqueEdges.emplace(a, b);

                    reverse = std::find(uniqueEdges.begin(), uniqueEdges.end(), std::make_pair(c, b));
                    if (reverse != uniqueEdges.end()) uniqueEdges.erase(reverse);
                    else uniqueEdges.emplace(b, c);

                    reverse = std::find(uniqueEdges.begin(), uniqueEdges.end(), std::make_pair(a, c));
                    if (reverse != uniqueEdges.end()) uniqueEdges.erase(reverse);
                    else uniqueEdges.emplace(c, a);

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

    void Solver::ResolveCollision(Physics& physicsA, Transform& transformA, Physics& physicsB, Transform& transformB, const CollisionInfo& collision)
    {
        // Calculate relative positions from center of mass to contact point.
        Vector3 relativeA = collision.contactPointA - transformA.GetPosition();
        Vector3 relativeB = collision.contactPointB - transformB.GetPosition();
        
        // Calculate velocity at contact point (linear + angular contribution).
        Vector3 angularVelocityA = Cross(physicsA.GetAngularVelocity(), relativeA);
        Vector3 angularVelocityB = Cross(physicsB.GetAngularVelocity(), relativeB);
        
        Vector3 fullVelocityA = physicsA.GetVelocity() + angularVelocityA;
        Vector3 fullVelocityB = physicsB.GetVelocity() + angularVelocityB;
        
        Vector3 relativeVelocity = fullVelocityB - fullVelocityA;
        float velocityAlongNormal = Dot(relativeVelocity, collision.normal);
        
        // Calculate angular effect on impulse (only for non-stationary objects).
        Vector3 inertiaA = Vector3(0.0f);
        Vector3 inertiaB = Vector3(0.0f);
        Matrix3 worldInverseInertiaTensorA = Matrix3(0.0f);
        Matrix3 worldInverseInertiaTensorB = Matrix3(0.0f);
        
        if (!physicsA.IsStationary())
        {
            worldInverseInertiaTensorA = physicsA.GetCollider().GetWorldInverseInertiaTensor(transformA, physicsA.GetMass());
            inertiaA = Cross(worldInverseInertiaTensorA * Cross(relativeA, collision.normal), relativeA);
        }
        if (!physicsB.IsStationary())
        {
            worldInverseInertiaTensorB = physicsB.GetCollider().GetWorldInverseInertiaTensor(transformB, physicsB.GetMass());
            inertiaB = Cross(worldInverseInertiaTensorB * Cross(relativeB, collision.normal), relativeB);
        }
        float angularEffect = Dot(inertiaA + inertiaB, collision.normal);
        
        // Calculate normal impulse magnitude.
        float e = Min(physicsA.GetRestitution(), physicsB.GetRestitution());
        float j = -(1.0f + e) * velocityAlongNormal;
        j /= (physicsA.GetInverseMass() + physicsB.GetInverseMass() + angularEffect);
        
        Vector3 impulse = j * collision.normal;
        
        // Apply linear impulses.
        if (!physicsA.IsStationary()) physicsA.ApplyLinearImpulse(-impulse);
        if (!physicsB.IsStationary()) physicsB.ApplyLinearImpulse(impulse);
        
        // Apply angular impulses.
        if (!physicsA.IsStationary()) physicsA.ApplyAngularImpulse(Cross(relativeA, -impulse), worldInverseInertiaTensorA);
        if (!physicsB.IsStationary()) physicsB.ApplyAngularImpulse(Cross(relativeB, impulse), worldInverseInertiaTensorB);
        
        // Calculate and apply friction impulses.
        Vector3 tangent = relativeVelocity - velocityAlongNormal * collision.normal;
        float tangentLength = Length(tangent);
        
        if (tangentLength > 0.0001f) // Avoid division by zero
        {
            tangent /= tangentLength; // Normalize tangent
            
            // Calculate angular effect for tangent direction.
            Vector3 inertiaTangentA = Vector3(0.0f);
            Vector3 inertiaTangentB = Vector3(0.0f);
            
            if (!physicsA.IsStationary())
            {
                inertiaTangentA = Cross(worldInverseInertiaTensorA * Cross(relativeA, tangent), relativeA);
            }
            if (!physicsB.IsStationary())
            {
                inertiaTangentB = Cross(worldInverseInertiaTensorB * Cross(relativeB, tangent), relativeB);
            }
            float angularEffectTangent = Dot(inertiaTangentA + inertiaTangentB, tangent);
            
            // Calculate friction impulse magnitude.
            float jt = -Dot(relativeVelocity, tangent);
            jt /= (physicsA.GetInverseMass() + physicsB.GetInverseMass() + angularEffectTangent);
            
            // Apply Coulomb friction (static vs dynamic).
            float mu = (physicsA.GetFriction() + physicsB.GetFriction()) * 0.5f;
            Vector3 frictionImpulse;
            
            if (Abs(jt) < j * mu)
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
            if (!physicsA.IsStationary()) physicsA.ApplyAngularImpulse(Cross(relativeA, -frictionImpulse), worldInverseInertiaTensorA);
            if (!physicsB.IsStationary()) physicsB.ApplyAngularImpulse(Cross(relativeB, frictionImpulse), worldInverseInertiaTensorB);
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
        deltaTime = Clamp(deltaTime, 0.0f, 0.016f);
        auto view = world.View<Transform, Physics>();
        for (auto [handle, transform, physics] : view)
        {
            if (physics.IsStationary()) continue;

            physics.ApplyForce(m_Gravity * physics.GetMass() * Vector3(0.0f, 0.0f, -1.0f));
            
            Matrix3 worldInverseInertiaTensor = physics.GetCollider().GetWorldInverseInertiaTensor(transform, physics.GetMass());
            physics.Integrate(worldInverseInertiaTensor, deltaTime);

            transform.TranslateBy(physics.GetVelocity() * deltaTime);
            
            Vector3 angularVelocity = physics.GetAngularVelocity();
            float deltaAngle = Length(angularVelocity) * deltaTime;
            if (deltaAngle > 1e-6f)
            {
                Vector3 rotationAxis = Normalized(angularVelocity);
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
                // else
                // {
                //     if (collision.status == CollisionInfo::Status::GJKFailed) std::cout << std::format("GJK failed between object {} and {}\n", (int) handleA, (int) handleB);
                //     else if (collision.status == CollisionInfo::Status::EPAFailed) std::cout << std::format("EPA failed between object {} and {}\n", (int) handleA, (int) handleB);
                // }
            }
        }
    }
}