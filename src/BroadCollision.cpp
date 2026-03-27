#include <h/BroadCollision.h>


BoundingSphere::BoundingSphere(const glm::vec3 &centre, float radius)
{
    BoundingSphere::centre = centre;
    BoundingSphere::radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere &one,
    const BoundingSphere &two)
{
    glm::vec3 centreOffset = two.centre - one.centre;
    float distance = glm::length(centreOffset);
    float radiusDiff = two.radius - one.radius;

    // If one sphere completely contains the other, return the larger.
    if (radiusDiff >= distance)
    {
        // 'two' encloses 'one'
        centre = two.centre;
        radius = two.radius;
        return;
    }
    if (-radiusDiff >= distance)
    {
        // 'one' encloses 'two'
        centre = one.centre;
        radius = one.radius;
        return;
    }

    // Otherwise we need to work with partially overlapping spheres
    radius = (distance + one.radius + two.radius) * 0.5f;

    // The new centre is based on one's centre, moved towards
    // two's centre by an amount proportional to the spheres' radii.
    if (distance > 0.0f)
    {
        centre = one.centre + centreOffset * ((radius - one.radius) / distance);
    }
    else
    {
        // centres are effectively identical
        centre = one.centre;
    }
}

bool BoundingSphere::overlaps(const BoundingSphere &other) const
{
    // Use squared distance to avoid unnecessary sqrt and compare correctly.
    glm::vec3 diff = centre - other.centre;
    float distSq = glm::dot(diff, diff);
    float rsum = radius + other.radius;
    return distSq < (rsum * rsum);
}

float BoundingSphere::getGrowth(const BoundingSphere &other) const
{
    BoundingSphere newSphere(*this, other);

    // We return a value proportional to the change in surface
    // area of the sphere.
    return newSphere.radius * newSphere.radius - radius * radius;
}

bool BVHNode::isLeaf() const
{
    return (body_id != INVALID_ID);
}

unsigned BVHNode::getPotentialContacts(PotentialContact* contacts, unsigned limit) const
{
    if (isLeaf() || limit == 0) return 0;

    // Test children against each other
    unsigned count = children[0]->getPotentialContactsWith(children[1], contacts, limit);

    // Also recurse within each child's subtree
    if (limit > count)
        count += children[0]->getPotentialContacts(contacts + count, limit - count);

    if (limit > count)
        count += children[1]->getPotentialContacts(contacts + count, limit - count);

    return count;
}

void BVHNode::insert(EntityID new_body_id, const BoundingSphere &newVolume)
{
    // If we are a leaf, then the only option is to spawn two
    // new children and place the new body in one.
    if (isLeaf())
    {
        // Child one is a copy of us.
        children[0] = new BVHNode(this, volume, body_id);

        // Child two holds the new body
        children[1] = new BVHNode(this, newVolume, new_body_id);

        // And we now loose the body (we're no longer a leaf)
        this->body_id = INVALID_ID;

        // We need to recalculate our bounding volume
        recalculateBoundingVolume();
    }

    // Otherwise we need to work out which child gets to keep
    // the inserted body. We give it to whoever would grow the
    // least to incorporate it.
    else
    {
        if (children[0]->volume.getGrowth(newVolume) <
            children[1]->volume.getGrowth(newVolume))
        {
            children[0]->insert(new_body_id, newVolume);
        }
        else
        {
            children[1]->insert(new_body_id, newVolume);
        }
    }
}

BVHNode::~BVHNode()
{
    // If we don't have a parent, then we ignore the sibling
    // processing
    if (parent)
    {
        // Find our sibling
        BVHNode *sibling;
        if (parent->children[0] == this) sibling = parent->children[1];
        else sibling = parent->children[0];

        // Write its data to our parent
        parent->volume = sibling->volume;
        parent->body_id = sibling->body_id;
        parent->children[0] = sibling->children[0];
        parent->children[1] = sibling->children[1];

        if (parent->children[0]) parent->children[0]->parent = parent;
        if (parent->children[1]) parent->children[1]->parent = parent;


        // Delete the sibling (we blank its parent and
        // children to avoid processing/deleting them)
        sibling->parent = NULL;
        sibling->body_id = INVALID_ID;
        sibling->children[0] = NULL;
        sibling->children[1] = NULL;
        delete sibling;

        // Recalculate the parent's bounding volume
        parent->recalculateBoundingVolume();
    }

    // Delete our children (again we remove their
    // parent data so we don't try to process their siblings
    // as they are deleted).
    if (children[0]) {
        children[0]->parent = NULL;
        delete children[0];
    }
    if (children[1]) {
        children[1]->parent = NULL;
        delete children[1];
    }

    //printf("Deleted/moved node %I64d\n", this);

}

bool BVHNode::overlaps(const BVHNode *other) const
{
    return volume.overlaps(other->volume);
}

void BVHNode::recalculateBoundingVolume(bool recurse)
{
    if (isLeaf()) return;
    // Use the bounding volume combining constructor.
    volume = BoundingSphere(children[0]->volume, children[1]->volume);
    // Recurse up the tree
    if (parent) parent->recalculateBoundingVolume(true);
}

unsigned BVHNode::getPotentialContactsWith(const BVHNode *other, PotentialContact *contacts, unsigned limit) const
{
    // Early out if we don't overlap or if we have no room
    // to report contacts
    if (!overlaps(other) || limit == 0) return 0;

    // If we're both at leaf nodes, then we have a potential contact
    if (isLeaf() && other->isLeaf())
    {
        contacts->body_id[0] = body_id;
        contacts->body_id[1] = other->body_id;
        return 1;
    }

    // Determine which node to descend into. If either is
    // a leaf, then we descend the other. If both are branches,
    // then we use the one with the largest size.
    if (other->isLeaf() ||
        (!isLeaf() && volume.getSize() >= other->volume.getSize()))
    {
        // Recurse into ourself
        unsigned count = children[0]->getPotentialContactsWith(
            other, contacts, limit
        );

        // Check we have enough slots to do the other side too
        if (limit > count) {
            return count + children[1]->getPotentialContactsWith(
                other, contacts + count, limit - count
            );
        }
        else {
            return count;
        }
    }
    else
    {
        // Recurse into the other node
        unsigned count = getPotentialContactsWith(
            other->children[0], contacts, limit
        );

        // Check we have enough slots to do the other side too
        if (limit > count) {
            return count + getPotentialContactsWith(
                other->children[1], contacts + count, limit - count
            );
        }
        else {
            return count;
        }
    }
}
