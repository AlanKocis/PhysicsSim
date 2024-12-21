#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <h/Transform.h>

class RigidBody
{
protected:
	int object_type{ 0 };
	float inverse_mass{ 0.0F };
	float linear_damping{ 0.0F };
	glm::vec3 velocity{ 0.0F, 0.0F, 0.0F };
	glm::vec3 acceleration{ 0.0F, 0.0F, 0.0F };
	glm::vec3 sum_forces{ 0.0F, 0.0F, 0.0F };
	glm::vec3 sum_torques{ 0.0F, 0.0F, 0.0F };
	glm::mat3 inverse_inertia_tensor;
	Transform transform;
public:
	void integrate(float time);
	bool hasInfiniteMass();
	void addForceAtCenter(const glm::vec3 &force);
	void addForceAtWorldPoint(const glm::vec3 &force, const glm::vec3 &point);
	void addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point);
	void setPosition(const glm::vec3 &position);
	glm::vec3 getPosition();
	glm::vec3 getVelocity();
	glm::vec3 getAcceleration();
	void setMass(const float &mass);
	float getInverseMass();
	float getMass();		// !!CHECK WITH Particle::hasInfiniteMass() BEFORE CALLING!!
	void setDamping(const float &d);
	float getDamping();
};






#endif