#include <h/ContactResolver.h>
#include <glm/glm.hpp>
#include <algorithm>

static void positionalCorrection(RigidBody& A, RigidBody& B, const glm::vec3& normal, float penetration)
{
    const float percent = 0.8f;
    const float slop = 0.001f;
    float invMassSum = A.inverseMass + B.inverseMass;
    if (invMassSum <= 0.0f) return;
    float correctionMag = std::max(penetration - slop, 0.0f) / invMassSum;
    glm::vec3 correction = correctionMag * percent * normal;
    if (A.inverseMass > 0.0f)
        A.transform.pos -= correction * A.inverseMass;
    if (B.inverseMass > 0.0f)
        B.transform.pos += correction * B.inverseMass;
}

static void applyImpulse(RigidBody& A, RigidBody& B, const glm::vec3& impulse, const glm::vec3& rA, const glm::vec3& rB)
{
    if (A.inverseMass > 0.0f) {
        A.velocity -= impulse * A.inverseMass;
        // BUG FIX: was -= , angular velocity for A must be +=
        A.angularVelocity += A.inverseInertiaTensorWorld * glm::cross(rA, impulse);
    }
    if (B.inverseMass > 0.0f) {
        B.velocity += impulse * B.inverseMass;
        B.angularVelocity += B.inverseInertiaTensorWorld * glm::cross(rB, impulse);
    }
}
void ResolveContact(RigidBody& A, RigidBody& B, const Contact& c)
{
    if (A.inverseMass <= 0.0f && B.inverseMass <= 0.0f)
        return;

    // always correct position regardless of velocity
    positionalCorrection(A, B, c.normal, c.penetration);

    glm::vec3 rA = c.point - A.transform.pos;
    glm::vec3 rB = c.point - B.transform.pos;
    glm::vec3 vA = A.velocity + glm::cross(A.angularVelocity, rA);
    glm::vec3 vB = B.velocity + glm::cross(B.angularVelocity, rB);
    glm::vec3 relVel = vB - vA;
    float velAlongNormal = glm::dot(relVel, c.normal);

    if (velAlongNormal > -0.01f)
        return;

    float invMassSum = A.inverseMass + B.inverseMass;
    glm::vec3 rAxn = glm::cross(rA, c.normal);
    glm::vec3 rBxn = glm::cross(rB, c.normal);
    float rotTerm = glm::dot(c.normal,
        glm::cross(A.inverseInertiaTensorWorld * rAxn, rA) +
        glm::cross(B.inverseInertiaTensorWorld * rBxn, rB));
    float denom = std::max(invMassSum + rotTerm, 1e-6f);
    float j = -(1.0f + c.restitution) * velAlongNormal / denom;
    j = glm::min(j, 50.0f);
    glm::vec3 normalImpulse = j * c.normal;
    applyImpulse(A, B, normalImpulse, rA, rB);

    vA = A.velocity + glm::cross(A.angularVelocity, rA);
    vB = B.velocity + glm::cross(B.angularVelocity, rB);
    relVel = vB - vA;
    glm::vec3 tangent = relVel - glm::dot(relVel, c.normal) * c.normal;
    float tlen = glm::length(tangent);
    if (tlen > 1e-6f) tangent /= tlen;
    else tangent = glm::vec3(0.0f);
    glm::vec3 rAt = glm::cross(rA, tangent);
    glm::vec3 rBt = glm::cross(rB, tangent);
    float rotTermT = glm::dot(tangent,
        glm::cross(A.inverseInertiaTensorWorld * rAt, rA) +
        glm::cross(B.inverseInertiaTensorWorld * rBt, rB));
    float denomT = std::max(invMassSum + rotTermT, 1e-6f);
    float jt = -glm::dot(relVel, tangent) / denomT;
    float maxFriction = c.friction * j;
    if (fabs(jt) > fabs(maxFriction))
        jt = (jt > 0.0f) ? fabs(maxFriction) : -fabs(maxFriction);
    glm::vec3 frictionImpulse = jt * tangent;
    applyImpulse(A, B, frictionImpulse, rA, rB);
}