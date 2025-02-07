#ifndef ENTITY_H
#define ENTITY_H

#include <h/RigidBody.h>

enum EntityID
{
	CUBE, PLANE, 
	NUM_ENTITIES
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


#endif