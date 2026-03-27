#ifndef ENTITY_H
#define ENTITY_H

#include <h/RigidBody.h>
#include <stdint.h>
#include <vector>
#include <h/Defaults.h>

typedef uint64_t EntityID;
#define INVALID_ID UINT64_MAX
#define PLANE_T 0.001f

enum ENTITY_TYPES
{
	CUBE,
	PLANE,
	NUM_ENTITY_TYPES
};

struct PlaneData
{
	glm::vec3 normal;
	float offset;
	glm::vec2 extents;   // half-lengths
};

struct SphereData
{
	float radius;
};

struct CubeData
{
	glm::vec3 extents;	// half-lengths
};

struct ShapeData
{
	ENTITY_TYPES type;
	union
	{
		PlaneData plane;
		SphereData sphere;
		CubeData cube;
	};
};

class EntityManager
{
private:
	static EntityID id_gen;
	static std::vector<EntityID> free_list;
	static size_t free_list_count;
	//static std::vector<EntityID> active_entities;
	//static size_t num_active_entities;
public:
	EntityManager();
	static EntityID GenEntityID();
	static void ResetIDs();
	//static int GetNumActiveEntities();
	static void RecycleEntityID(EntityID id);	// adds id to free list
};

template<typename T>
struct EntityComponentBuffer
{
	std::vector<T> components;			//dense component array
	std::vector<EntityID> lookup;		//sparse lookup array (entityID->dense index)
	std::vector<EntityID> owner;		//stores the entityIDs of components stored at same index as dense array
	size_t size = 0;

	void AddComponent(EntityID id, const T& component);
	void RemoveComponent(EntityID id);
	void FreeReallocBuffers();
	bool Exists(EntityID id);
	T &operator[](EntityID id);
	EntityComponentBuffer();
};

template<typename T>
inline void EntityComponentBuffer<T>::AddComponent(EntityID id, const T &component)
{
	assert(id < INVALID_ID);
	assert(id < ENTITY_ALLOC_COUNT, "entity ID out of range");
	assert(id >= 0, "invalid entity ID");
	components.emplace_back(component);
	owner.emplace_back(id);
	lookup[id] = components.size() - 1;

	size++;
}

template<typename T>
inline void EntityComponentBuffer<T>::RemoveComponent(EntityID id)
{
	assert(id < ENTITY_ALLOC_COUNT, "entity ID out of range");
	assert(id >= 0, "invalid negative entity ID");
	//assert(id <= size, "invalid overflow entity ID");

	size_t dense_idx = lookup[id];	//dense idx of id to be removed (element from back swapped to this index)
	if (dense_idx == INVALID_ID)
	{
		printf("invalid entity ID\n");
		return;
	}
	
	size_t last_dense_idx = this->size - 1;	//index in dense array of last element
	EntityID last_dense_owner = owner[last_dense_idx];	//index in sparse array of element swapped from back of dense array

	std::swap(components[dense_idx], components[last_dense_idx]);
	std::swap(owner[dense_idx], owner[last_dense_idx]);
	lookup[last_dense_owner] = dense_idx;
	lookup[id] = INVALID_ID;

	size--;
	components.pop_back();
	owner.pop_back();

}

template<typename T>
inline void EntityComponentBuffer<T>::FreeReallocBuffers()
{
	size = 0;
	components.clear();
	lookup.clear();
	owner.clear();

	components.reserve(ENTITY_ALLOC_COUNT);
	owner.reserve(ENTITY_ALLOC_COUNT);

	lookup.resize(ENTITY_ALLOC_COUNT);
	lookup.assign(ENTITY_ALLOC_COUNT, INVALID_ID);
}

template<typename T>
inline bool EntityComponentBuffer<T>::Exists(EntityID id)
{
	if (id > ENTITY_ALLOC_COUNT)
		return false;
	else if (lookup[id] != INVALID_ID)
		return true;
	else
		return false;
}

template<typename T>
inline T &EntityComponentBuffer<T>::operator[](EntityID id)
{
	assert(id < ENTITY_ALLOC_COUNT && "entity ID out of range");
	assert(id >= 0, "Invalid entity ID");
	assert(lookup[id] != INVALID_ID, "entity does not hold component type");

	return components[lookup[id]];
}

template<typename T>
inline EntityComponentBuffer<T>::EntityComponentBuffer()
{
	FreeReallocBuffers();
}


/* NOT USING THIS ANYMORE, USE ENTITYCOMPONENTBUFFERS*/
struct CubeEntity
{
	//	main internals
	RigidBody physics;
	uint16_t entity_id;
	uint32_t vao_id;
	uint32_t shader_id;
	// others
	//glm::vec3 color;
	CubeEntity();
};

struct PlaneEntity
{
	RigidBody physics;
	uint32_t vao_id;
	uint32_t shader_id;
	PlaneEntity();
};

struct RenderComponent
{
	uint32_t vao_id;
	uint32_t shader_id;
	bool should_render = 1;
};

struct InstancedRenderComponent
{
	uint16_t vao_id;
	uint16_t shader_id;
};

#endif

