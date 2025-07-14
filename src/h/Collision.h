#ifndef COLLISION_H
#define COLLISION_H

#include <glm/common.hpp>
#include <h/Defaults.h>
#include <h/StaticPool.h>
#include <h/Entity.h>
#include <algorithm>
#include <math.h>

//
//	This is an implementation of a dynamic bounding volume hierarchy by Erin Catto
//	https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
//

struct AABB
{
	glm::vec3 lower_bound;
	glm::vec3 upper_bound;

	AABB Union(const AABB &l_box, const AABB &r_box);
	float Area(const AABB &box);
};

struct AABB_Node
{
	AABB box;
	EntityID object_id;
	int parent_index;
	int child_1;
	int child_2;
	bool is_leaf;
};

class AABB_Tree
{
private:
	hobPool<AABB_Node> node_pool;
	int node_count;
	int root_index;

public:
	AABB_Tree();
	inline int AllocLeafNode(EntityID object_id, const AABB &box);
	void InsertLeaf(EntityID object_id, const AABB &box);


};

















#endif