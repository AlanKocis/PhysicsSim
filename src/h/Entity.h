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
	uint32_t vaoID;
	uint32_t shaderID;
	// others
	//glm::vec3 color;
	CubeEntity();
};

struct PlaneEntity
{
	RigidBody physics;
	uint32_t vaoID;
	uint32_t shaderID;
	PlaneEntity();
};


#endif