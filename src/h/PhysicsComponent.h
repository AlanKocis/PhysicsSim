#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

enum PHYSICS_COMPONENT_ID
{
	PARTICLE
};


class PhysicsComponent
{
private:
	PHYSICS_COMPONENT_ID ID;
public:
	int getID();
};



#endif 