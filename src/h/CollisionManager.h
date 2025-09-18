#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <h/MemoryPool.h>
#include <h/BroadCollision.h>
#include <iostream>

#define MAX_CONTACTS 256


struct Contact
{
	EntityID body_id[2];
	float friction;
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