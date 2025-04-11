#ifndef ENTITY_H
#define ENTITY_H

#include <h/RigidBody.h>
#include <stdint.h>
#include <vector>
#include <h/Defaults.h>

typedef uint64_t EntityID;
#define INVALID_INDEX -1

class EntityManager
{
private:
	static EntityID id_gen;
public:
	static EntityID GenEntityID();
	static void ResetIDs();
	static int GetNumActiveEntities();
};

template<typename T>
struct EntityComponentBuffer
{
	std::vector<T> components;			//dense
	std::vector<EntityID> lookup;		//sparse

	void AddComponent(EntityID id, const T& component);
	void FreeReallocBuffers();
	T &operator[](EntityID id);
	EntityComponentBuffer();
};

template<typename T>
inline void EntityComponentBuffer<T>::AddComponent(EntityID id, const T &component)
{
	assert(id < ENTITY_ALLOC_COUNT, "entity ID out of range");
	assert(id >= 0, "invalid entity ID");
	components.emplace_back(component);
	lookup[id] = components.size() - 1;
}

template<typename T>
inline void EntityComponentBuffer<T>::FreeReallocBuffers()
{
	components.clear();
	lookup.clear();

	components.reserve(ENTITY_ALLOC_COUNT);
	lookup.resize(ENTITY_ALLOC_COUNT);
	lookup.assign(ENTITY_ALLOC_COUNT, INVALID_INDEX);
}


template<typename T>
inline T &EntityComponentBuffer<T>::operator[](EntityID id)
{
	assert(id < ENTITY_ALLOC_COUNT && "entity ID out of range");
	assert(id >= 0, "Invalid entity ID");
	assert(lookup[id] != INVALID_INDEX, "entity does not hold component type");

	return components[lookup[id]];
}

template<typename T>
inline EntityComponentBuffer<T>::EntityComponentBuffer()
{
	FreeReallocBuffers();
}

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
