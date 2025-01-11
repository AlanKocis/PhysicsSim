#include <h/RigidBody.h>

RigidBody::RigidBody()
{
	orientation = { 0, 0, 0, 0 };
	velocity = { 0, 0, 0 };
	angularVelocity = { 0, 0, 0 };
	acceleration = { 0, 0, 0 };
	sumForces = { 0, 0, 0 };
	sumTorques = { 0, 0, 0 };
	inverseMass = 0.1f;
	angularDamping = 0.7f;
	linearDamping = 0.7f;
	shouldRender = true;
	inverseInertiaTensor = glm::mat3(1.0f);
	inverseInertiaTensorWorld = glm::mat3(1.0f);
}

void RigidBody::integrate(float time)
{
	if (this->inverseMass <= 0.0F)
		return;
	assert(time > 0.0F);

	glm::vec3 p = transform.pos;

	glm::vec3 lastFrameAcceleration = acceleration;
	lastFrameAcceleration += sumForces * inverseMass;

	glm::vec3 angularAcceleration = inverseInertiaTensorWorld * sumTorques;

	velocity += lastFrameAcceleration * time;
	angularVelocity += angularAcceleration * time;
	velocity *= powf(linearDamping, time);
	angularVelocity *= powf(linearDamping, time);

	transform.pos += velocity * time;
	//orientation += angularVelocity * time * 0.5f;
	glm::quat q(0, angularVelocity.x, angularVelocity.y, angularVelocity.z);
	orientation += (q * time * 0.5f);
	glm::normalize(orientation);
	
	sumForces = { 0, 0, 0 };
	sumTorques = { 0, 0, 0 };
}

/*
void RigidBody::updateMatrices()
{
	if (inverseMass == 0)
		return;



	switch (entityType)
	{
	case CUBE:
//		calculate inertia tensor
//
		float x = (transform.scale.x * transform.scale.x);
		float y = (transform.scale.y * transform.scale.y);
		float z = (transform.scale.z * transform.scale.z);

		glm::mat3 tensor;
		tensor[0][0] = (1 / 12) * (1 / inverseMass) * (y + z);
		tensor[0][1] = 0;
		tensor[0][2] = 0;
		tensor[1][0] = 0;
		tensor[1][1] = (1 / 12) * (1 / inverseMass) * (x + z);
		tensor[1][0] = 0;
		tensor[2][0] = 0;
		tensor[2][1] = 0;
		tensor[2][2] = (1 / 12) * (1 / inverseMass) * (x + y);

		inverseInertiaTensor = glm::inverse(tensor);
//		Transform basis to get world coordinate inertia tensor
//
		glm::mat3 iM = glm::inverse(glm::mat3(transform.worldMatrix));
		iM = (inverseInertiaTensor * iM);
		inverseInertiaTensorWorld = glm::mat3(transform.worldMatrix) * iM;


		break;
	}




}
*/

bool RigidBody::hasInfiniteMass()
{
	if (this->inverseMass <= 0.0F)
		return true;
	else
		return false;
}

void RigidBody::addForceAtCenter(const glm::vec3 &force)
{
}

void RigidBody::addForceAtWorldPoint(const glm::vec3 &force, const glm::vec3 &point)
{
}

void RigidBody::addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point)
{
}



void RigidBody::setPosition(const glm::vec3 &position)
{
}

glm::vec3 RigidBody::getPosition()
{
	return this->transform.pos;
}

glm::vec3 RigidBody::getVelocity()
{
	return this->velocity;
}

glm::vec3 RigidBody::getAcceleration()
{
	return this->acceleration;
}

void RigidBody::setMass(const float &mass)
{
	if (mass <= 0.0F)
	{
		this->inverseMass = 0.0F;
		return;
	}
	this->inverseMass = (1.0F / mass);
}

float RigidBody::getInverseMass()
{
	return this->inverseMass;
}

// !!CHECK WITH Particle::hasInfiniteMass() BEFORE CALLING!!
float RigidBody::getMass()
{
	return (1.0F / this->inverseMass);
}

void RigidBody::setDamping(const float &d)
{
	this->linearDamping = d;
}

float RigidBody::getLinearDamping()
{
	return this->linearDamping;
}

float RigidBody::getAngularDamping()
{
	return this->angularDamping;
}

void RigidBody::clearForces()
{
}

void RigidBody::clearTorques()
{

}