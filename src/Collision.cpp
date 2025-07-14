#include <h/Collision.h>


	AABB AABB::Union(const AABB &l_box, const AABB &r_box)
	{
		AABB result;
		//	 broken logic - returning vec3 result from std::min.
		result.lower_bound = {
			std::min(l_box.lower_bound.x, r_box.lower_bound.x),
			std::min(l_box.lower_bound.y, r_box.lower_bound.y),
			std::min(l_box.lower_bound.z, r_box.lower_bound.z)
		};

		result.upper_bound = {
			std::max(l_box.upper_bound.x, r_box.upper_bound.x),
			std::max(l_box.upper_bound.y, r_box.upper_bound.y),
			std::max(l_box.upper_bound.z, r_box.upper_bound.z)
		};

		return result;
	}


float AABB::Area(const AABB &box)
{
	glm::vec3 d = box.upper_bound - box.lower_bound;
	return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
}





AABB_Tree::AABB_Tree()
{
	node_count = 0;
	root_index = 0;
}

int AABB_Tree::AllocLeafNode(EntityID object_id, const AABB &box)
{
	int index = node_pool.alloc();
	node_pool.buffer[index].box = box;
	node_pool.buffer[index].object_id = object_id;
	node_pool.buffer[index].is_leaf = -1;
	node_pool.buffer[index].child_1 = -1;
	node_pool.buffer[index].child_2 = -1;
}

/*
void AABB_Tree::InsertLeaf(EntityID object_id, const AABB &box)
{

	int leaf_index = AllocLeafNode(object_id, box);
	if (node_count == 1)
	{
		root_index = leaf_index;
	}



	// find best sibling



	// create new parent



	// walk up and refit tree




}
*/