#ifndef BROAD_COLLISION_H
#define BROAD_COLLISION_H

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <h/Transform.h>
#include <h/RigidBody.h>
#include <h/Defaults.h>
#include <h/Entity.h>

struct PotentialContact
{
	EntityID body_id[2];
};

struct BoundingSphere
{
	glm::vec3 centre;
	float radius;

	BoundingSphere(const glm::vec3 &centre, float radius);
	BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);
	bool overlaps(const BoundingSphere &other) const;
	float getGrowth(const BoundingSphere &other) const;
	float getSize() const
	{
		return (1.333333f) * 3.14 * radius * radius * radius;
	}
};

struct PlaneCollider
{
    glm::vec3 normal;
    float offset;
	glm::vec2 extents;   // half-lengths
};

/**
 * A base class for nodes in a bounding volume hierarchy.
 *
 * This class uses a binary tree to store the bounding
 * volumes.
 */

class BVHNode
{
public:
    /**
     * Holds the child nodes of this node.
     */
    BVHNode *children[2];

    /**
     * Holds a single bounding volume encompassing all the
     * descendents of this node.
     */
    BoundingSphere volume;

    /**
     * Holds the rigid body at this node of the hierarchy.
     * Only leaf nodes can have a rigid body defined (see isLeaf).
     * Note that it is possible to rewrite the algorithms in this
     * class to handle objects at all levels of the hierarchy,
     * but the code provided ignores this vector unless firstChild
     * is NULL.
     */
    EntityID body_id;   //max u64 if "null"

    // ... other BVHNode code as before ...

    /**
     * Holds the node immediately above us in the tree.
     */
    BVHNode *parent;

    /**
     * Creates a new node in the hierarchy with the given parameters.
     */
    BVHNode(BVHNode *parent, const BoundingSphere &volume,
        EntityID body_id = INVALID_ID)
        : parent(parent), volume(volume), body_id(body_id)
    {
    
        children[0] = children[1] = NULL;
    }

    /**
     * Checks if this node is at the bottom of the hierarchy.
     */
    bool isLeaf() const;

    /**
     * Checks the potential contacts from this node downwards in
     * the hierarchy, writing them to the given array (up to the
     * given limit). Returns the number of potential contacts it
     * found.
     */
    unsigned getPotentialContacts(PotentialContact *contacts,
        unsigned limit) const;

    /**
     * Inserts the given rigid body, with the given bounding volume,
     * into the hierarchy. This may involve the creation of
     * further bounding volume nodes.
     */
    void insert(EntityID new_body_id, const BoundingSphere &volume);

    /**
     * Deltes this node, removing it first from the hierarchy, along
     * with its associated
     * rigid body and child nodes. This method deletes the node
     * and all its children (but obviously not the rigid bodies). This
     * also has the effect of deleting the sibling of this node, and
     * changing the parent node so that it contains the data currently
     * in that sibling. Finally it forces the hierarchy above the
     * current node to reconsider its bounding volume.
     */
    ~BVHNode();

protected:

    /**
     * Checks for overlapping between nodes in the hierarchy. Note
     * that any bounding volume should have an overlaps method implemented
     * that checks for overlapping with another object of its own type.
     */
    bool overlaps(const BVHNode *other) const;

    /**
     * Checks the potential contacts between this node and the given
     * other node, writing them to the given array (up to the
     * given limit). Returns the number of potential contacts it
     * found.
     */
    unsigned getPotentialContactsWith(
        const BVHNode *other,
        PotentialContact *contacts,
        unsigned limit) const;

    /**
     * For non-leaf nodes, this method recalculates the bounding volume
     * based on the bounding volumes of its children.
     */
    void recalculateBoundingVolume(bool recurse = true);
};

// Note that, because we're dealing with a template here, we
// need to have the implementations accessible to anything that
// imports this header.




#endif