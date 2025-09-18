#include <h/CollisionManager.h>

void CollisionManager::ResetPContacts()
{
	pcontacts_left = MAX_CONTACTS;
	pcontacts_left = 0;
	pcontacts = pcontact_array;
}

void CollisionManager::AddPContact(size_t count)
{
	pcontacts_left -= count;
	pcontact_count += count;
	pcontacts += count;
}

bool CollisionManager::HasMorePContacts()
{
	return (pcontacts_left > 0);
}

void CollisionManager::ResetContacts()
{
	contacts_left = MAX_CONTACTS;
	contact_count = 0;
	contacts = contact_array;
}

void CollisionManager::AddContact(size_t count)
{
	contacts_left -= count;
	contact_count += count;
	contacts += count;
}

bool CollisionManager::HasMoreContacts()
{
	return (contacts_left > 0);
}

SphereTree::SphereTree()
{
	id_lookup.FreeReallocBuffers();
	root = nullptr;
}

SphereTree::~SphereTree()
{
	id_lookup.FreeReallocBuffers();
	Clear();
}

void SphereTree::AddEntity(EntityID id, const BoundingSphere &volume)
{
	if (!root)
	{
		root = new BVHNode(nullptr, volume, id);
	}
	else
	{
		root->insert(id, volume);
	}

	RebuildIDMap();

}

void SphereTree::RemoveEntity(EntityID id)
{
	if (!id_lookup.Exists(id))
		return;


	BVHNode *to_delete = id_lookup[id];
	if (to_delete == root)
	{
		delete to_delete;
		root = nullptr;
	}
	else
		delete to_delete;


	RebuildIDMap();
}

void SphereTree::Clear()
{
	id_lookup.FreeReallocBuffers();

	if (root)
	{
		delete root;
		root = nullptr;
	}


}

void SphereTree::PrintLeafIDs(const BVHNode *node)
{
	if (!node) return;

	if (node->isLeaf()) {
		std::cout << "Leaf Node ID: " << node->body_id << std::endl;
	}
	else {
		PrintLeafIDs(node->children[0]);
		PrintLeafIDs(node->children[1]);
	}
}

void SphereTree::UpdateEntity(EntityID id)
{




}

void SphereTree::RebuildIDMap()
{
	id_lookup.FreeReallocBuffers();
	if (root)
		TraverseMap(root);
}

void SphereTree::TraverseMap(BVHNode *node)
{
	if (!node) return;

	if (node->isLeaf()) {
		// Found a leaf - add to mapping
		id_lookup.AddComponent(node->body_id, node);
	}
	else {
		// Recursively traverse children
		TraverseMap(node->children[0]);
		TraverseMap(node->children[1]);
	}
}