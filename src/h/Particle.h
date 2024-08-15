#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <cassert>
#include <cmath>

#define INFINITE_MASS 0.0F

class Particle
{
protected:
	glm::vec3 position{ 0.0F, 0.0F, 0.0F };
	glm::vec3 velocity{ 0.0F, 0.0F, 0.0F };
	glm::vec3 acceleration{ 0.0F, 0.0F, 0.0F };
	glm::vec3 sumForces{ 0.0F, 0.0F, 0.0F };
	float damping{ 0.0F };
	float inverseMass{ 0.0F };
public:
	void integrate(float time);
	bool hasInfiniteMass();
	void addForce(const glm::vec3 &force);
	void setPosition(const glm::vec3 &position);
	glm::vec3 getPosition();
	void setMass(const float &mass);
	float getInverseMass();
	float getMass();
	void setDamping(const float &d);
	float getDamping();
private:
	void clearForces();
};
#endif