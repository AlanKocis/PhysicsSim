#include <h/NarrowCollision.h>
#include <h/Scene.h>
#include <h/CollisionManager.h>
#include <h/ContactResolver.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>

using namespace glm;

namespace NarrowPhase
{

    static inline void getAxesAndExtents(const Transform& t, vec3 axes[3], vec3& center, vec3& extents)
    {
        mat3 R = mat3_cast(t.orientation);
        axes[0] = normalize(R[0]);
        axes[1] = normalize(R[1]);
        axes[2] = normalize(R[2]);
        center = t.pos;
        extents = t.scale * 0.5f;
    }

    static float projectRadius(const vec3 axes[3], const vec3& extents, const vec3& L)
    {
        return extents.x * fabs(dot(axes[0], L))
            + extents.y * fabs(dot(axes[1], L))
            + extents.z * fabs(dot(axes[2], L));
    }

    // Returns the point on OBB (defined by axes/center/extents) closest to world-space point p.
    static vec3 closestPointOnOBB(const vec3& p, const vec3 axes[3], const vec3& center, const vec3& extents)
    {
        vec3 d = p - center;
        vec3 closest = center;
        for (int i = 0; i < 3; ++i)
        {
            float dist = dot(d, axes[i]);
            dist = glm::clamp(dist, -extents[i], extents[i]);
            closest += dist * axes[i];
        }
        return closest;
    }

    bool OBBvsOBB(const RigidBody& A, const RigidBody& B, ContactInfo& out)
    {
        vec3 A_axes[3], B_axes[3];
        vec3 Ac, Bc, Aext, Bext;
        getAxesAndExtents(A.transform, A_axes, Ac, Aext);
        getAxesAndExtents(B.transform, B_axes, Bc, Bext);

        vec3 d = Bc - Ac;

        vec3 candidateAxes[15];
        int na = 0;
        for (int i = 0; i < 3; ++i) candidateAxes[na++] = A_axes[i];
        for (int i = 0; i < 3; ++i) candidateAxes[na++] = B_axes[i];
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                candidateAxes[na++] = cross(A_axes[i], B_axes[j]);

        float minPen = std::numeric_limits<float>::infinity();
        vec3 minAxis(1, 0, 0);

        for (int k = 0; k < na; ++k)
        {
            vec3 L = candidateAxes[k];
            float len = length(L);
            if (len < 1e-6f) continue;
            L /= len;

            float ra = projectRadius(A_axes, Aext, L);
            float rb = projectRadius(B_axes, Bext, L);

            float distance = fabs(dot(d, L));
            float overlap = ra + rb - distance;
            if (overlap <= 0.0f)
                return false;

            if (overlap < minPen)
            {
                minPen = overlap;
                minAxis = L;
                if (dot(minAxis, d) < 0.0f) minAxis = -minAxis;
            }
        }

        out.penetration = minPen;
        out.normal = normalize(minAxis);

        // FIX: compute contact point as closest point on B's surface to A's center,
        // then closest point on A's surface to that — gives a point on or near
        // the actual contact surface instead of the midpoint-between-centers hack.
        vec3 supportB = closestPointOnOBB(Ac, B_axes, Bc, Bext);
        vec3 supportA = closestPointOnOBB(supportB, A_axes, Ac, Aext);
        out.point = (supportA + supportB) * 0.5f;

        return true;
    }

    bool OBBvsPlane(const RigidBody& box, const ShapeData& boxShape,
        const RigidBody& plane, const ShapeData& planeShape, ContactInfo& out)
    {
        vec3 boxAxes[3];
        vec3 boxCenter, boxExtents;
        getAxesAndExtents(box.transform, boxAxes, boxCenter, boxExtents);
        //boxExtents = boxShape.cube.extents;

        mat3 planeR = mat3_cast(plane.transform.orientation);
        vec3 planeNormal = normalize(planeR * planeShape.plane.normal);
        float planeD = dot(planeNormal, plane.transform.pos) + planeShape.plane.offset;

       // const float SKIN = 0.2f;
        float radius = projectRadius(boxAxes, boxExtents, planeNormal);
        float centerDist = dot(planeNormal, boxCenter) - planeD;
        if (centerDist < 0.0f) return false;  // box is below, ignore
        float penetration = radius - centerDist;

        if (penetration <= 0.0f) return false;

        // also reject if box is on the wrong side entirely
        if (centerDist < -radius) return false;

        vec3 contactPoint = boxCenter - planeNormal * radius;

        vec3 d = contactPoint - plane.transform.pos;
        float u = dot(d, planeR[0]);
        float v = dot(d, planeR[2]);
       // if (fabs(u) > planeShape.plane.extents.x) return false;
        //if (fabs(v) > planeShape.plane.extents.y) return false;

        out.normal = planeNormal;
        out.penetration = penetration;
        out.point = contactPoint;
        return true;
    }

    inline bool isPlane(const RigidBody& rb)
    {
     
    }

    void ProcessCollisions(Scene& scene)
    {
        scene.collision_data.ResetPContacts();
        scene.collision_data.ResetContacts();
        if (!scene.BVHTree.root) return;

        unsigned num_ctct = scene.BVHTree.root->getPotentialContacts(scene.collision_data.pcontacts, MAX_CONTACTS);
        for (unsigned pcIdx = 0; pcIdx < num_ctct; ++pcIdx)
        {
            PotentialContact& pc = scene.collision_data.pcontacts[pcIdx];
            EntityID a = pc.body_id[0];
            EntityID b = pc.body_id[1];

            if (!scene.BVHTree.id_lookup.Exists(a) || !scene.BVHTree.id_lookup.Exists(b))
                continue;

            RigidBody& ra = scene.rigid_body_components[a];
            RigidBody& rb = scene.rigid_body_components[b];
            ShapeData& sa = scene.shape_data_components[a];
            ShapeData& sb = scene.shape_data_components[b];

            ContactInfo ci; ci.a = a; ci.b = b;
            bool hit = false;

            if (sa.type == ENTITY_TYPES::CUBE && sb.type == ENTITY_TYPES::CUBE)
            {
                hit = OBBvsOBB(ra, rb, ci);
            }
            else if (sa.type == ENTITY_TYPES::CUBE && sb.type == ENTITY_TYPES::PLANE)
            {
                hit = OBBvsPlane(ra, sa, rb, sb, ci);
            }
            else if (sa.type == ENTITY_TYPES::PLANE && sb.type == ENTITY_TYPES::CUBE)
            {
                hit = OBBvsPlane(rb, sb, ra, sa, ci);
            }

            if (hit)
            {
                Contact& c = *scene.collision_data.contacts;
                c.body_id[0] = a; c.body_id[1] = b;
                c.point = ci.point; c.normal = ci.normal; c.penetration = ci.penetration;
                c.restitution = 0.0f;
                c.friction = 1.5f;
                scene.collision_data.AddContact(1);
            }
        }

        if (scene.collision_data.contact_count > 0)
        {
            for (size_t ci = 0; ci < scene.collision_data.contact_count; ++ci)
            {
                Contact& c = scene.collision_data.contact_array[ci];
                RigidBody& A = scene.rigid_body_components[c.body_id[0]];
                RigidBody& B = scene.rigid_body_components[c.body_id[1]];

                if (A.inverseMass > 0.0f)
                {
                    float vn = glm::dot(A.velocity, c.normal);
                    if (vn < 0.0f)
                        A.velocity -= vn * c.normal;

                    glm::vec3 tangential = A.velocity - glm::dot(A.velocity, c.normal) * c.normal;
                    A.velocity -= tangential * c.friction * 0.3f;
                }
                if (B.inverseMass > 0.0f)
                {
                    float vn = glm::dot(B.velocity, c.normal);
                    if (vn < 0.0f)
                        B.velocity -= vn * c.normal;

                    glm::vec3 tangential = B.velocity - glm::dot(B.velocity, c.normal) * c.normal;
                    B.velocity -= tangential * c.friction * 0.3f;
                }

                ResolveContact(A, B, c);
            }
        }
    }

} // namespace NarrowPhase