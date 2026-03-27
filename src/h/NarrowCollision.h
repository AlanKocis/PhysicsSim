#ifndef NARROW_PHASE_H
#define NARROW_PHASE_H

#include <glm/glm.hpp>
#include <h/RigidBody.h>
#include <h/BroadCollision.h>

struct Scene; // forward declare to avoid circular include

struct ContactInfo
{
    EntityID a;
    EntityID b;
    glm::vec3 point;
    glm::vec3 normal; // from a -> b
    float penetration;
};

namespace   NarrowPhase
{
    // Test two boxes (use RigidBody.transform.scale as full side lengths)
    // Returns true and fills out contact info if colliding.
    bool OBBvsOBB(const RigidBody& A, const RigidBody& B, ContactInfo& out);
    bool OBBvsPlane(const RigidBody& box, const ShapeData& boxShape,
        const RigidBody& plane, const ShapeData& planeShape, ContactInfo& out);

    // Run broad-phase (BVH) -> narrow-phase -> fill collision manager and resolve contacts.
    // Implemented in NarrowCollision.cpp
    void ProcessCollisions(Scene &scene);
}

#endif