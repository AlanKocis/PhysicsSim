#include <h/RigidBody.h>
/*
void RigidBody::integrate(float time)
{
	if (this->inverseMass <= 0.0F)
		return;
	assert(time > 0.0F);


	//FIX this later. EVIL CODE
	if (this->position.y < 0.0F)
	{
		this->position.y = 0.0F;
		this->velocity.y = 0.0F;

		//this->addForce(glm::vec3(0, 1000, 0));
	}

	glm::vec3 lastFrameAcceleration = this->acceleration;

	lastFrameAcceleration += (this->sumForces * this->inverseMass);

	//gravity
	if (this->position.y > 0.0f)
		this->acceleration += glm::vec3(0.0F, -10.0F, 0.0F);



	//	update position
	this->position = (this->position + (time * this->velocity));
	//	update velocity
	this->velocity = powf(this->damping, time) * this->velocity;
	this->velocity = (this->velocity + (time * this->acceleration));

	//	clear forces
	this->clearForces();
}



bool RigidBody::hasInfiniteMass()
{
	if (this->inverseMass <= 0.0F)
		return true;
	else
		return false;
}

void RigidBody::addForceAtCenter(const glm::vec3 &force)
{
	this->sumForces = sumForces + force;
}

void RigidBody::addForceAtWorldPoint(const glm::vec3 &force, const glm::vec3 &point)
{
}

void RigidBody::addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point)
{
}



void RigidBody::setPosition(const glm::vec3 &position)
{
	this->position = position;
}

glm::vec3 RigidBody::getPosition()
{
	return this->position;
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
	this->damping = d;
}

float RigidBody::getDamping()
{
	return this->damping;
}



void RigidBody::clearForces()
{
	this->sumForces = glm::vec3(0.0f, 0.0f, 0.0f);
}


*/