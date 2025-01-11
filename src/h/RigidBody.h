#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <h/Transform.h>

struct RigidBody
{
	Transform transform;
	glm::mat3 inverseInertiaTensor;
	glm::mat3 inverseInertiaTensorWorld;
	glm::quat orientation;
	glm::vec3 velocity;
	glm::vec3 angularVelocity;
	glm::vec3 acceleration; 
	glm::vec3 sumForces;
	glm::vec3 sumTorques;
	float inverseMass;
	float linearDamping;
	float angularDamping;
	bool shouldRender;
	RigidBody();
	//void loadRigidBody(EntityID id);
	void integrate(float time);
	//void updateMatrices();
	void setInertiaTensor();
	bool hasInfiniteMass();
	void addForceAtCenter(const glm::vec3 &force);
	void addForceAtWorldPoint(const glm::vec3 &force, const glm::vec3 &point);
	void addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point);
	void setPosition(const glm::vec3 &position);
	void setMass(const float &mass);
	void setDamping(const float &d);
	glm::vec3 getPosition();
	glm::vec3 getVelocity();
	glm::vec3 getAcceleration();
	float getMass();		// !!CHECK WITH Particle::hasInfiniteMass() BEFORE CALLING!!
	float getInverseMass();
	float getLinearDamping();
	float getAngularDamping();
	void clearForces();
	void clearTorques();
};






#endif