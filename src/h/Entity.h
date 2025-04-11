#ifndef ENTITY_H
#define ENTITY_H

#include <h/RigidBody.h>
#include <stdint.h>

typedef uint32_t EntityID;
#define INVALID_INDEX -1

class EntityManager
{
private:
	static EntityID id_gen;
public:
	static EntityID GenEntityID();
};

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
	uint16_t vao_id;
	uint16_t shader_id;
	bool should_render = 0;
};

struct InstancedRenderComponent
{
	uint16_t vao_id;
	uint16_t shader_id;
};

#endif