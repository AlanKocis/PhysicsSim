#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <h/MemoryPool.h>
#include <h/BroadCollision.h>
#include <glm/glm.hpp>
#include <iostream>

#define MAX_CONTACTS 8192

struct Contact
{
	EntityID body_id[2];

	// Contact frame (world space)
	glm::vec3 point;       // contact point on surface (world)
	glm::vec3 normal;      // unit normal pointing from body 0 -> body 1
	float penetration;     // penetration depth (positive => overlapping)

	// material/solver properties
	float restitution;     // bounciness (0..1)
	float friction;        // coefficient of friction

	// accumulators for iterative solvers (optional)
	float accumulatedNormalImpulse;
	float accumulatedTangentImpulse;
};

class SphereTree
{
public:
	EntityComponentBuffer<BVHNode *> id_lookup;
	BVHNode *root;
	SphereTree();
	~SphereTree();
	void AddEntity(EntityID id, const BoundingSphere &volume);
	void RemoveEntity(EntityID id);
	void Clear();
	void PrintLeafIDs(const BVHNode *node);
	void UpdateEntity(EntityID id);
private:
	void RebuildIDMap();
	void TraverseMap(BVHNode *node);
};

struct CollisionManager
{
//
	PotentialContact pcontact_array[MAX_CONTACTS];
	PotentialContact *pcontacts;
	size_t pcontacts_left;
	size_t pcontact_count;
//
	Contact contact_array[MAX_CONTACTS];
	Contact *contacts;
	size_t contacts_left;
	size_t contact_count;
	float friction;
//
	void ResetPContacts();
	void AddPContact(size_t count);
	bool HasMorePContacts();
	void ResetContacts();
	void AddContact(size_t count);
	bool HasMoreContacts();

};

#endif