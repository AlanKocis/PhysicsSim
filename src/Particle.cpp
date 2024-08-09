#include <h/Particle.h>

void Particle::integrate(float time)
{
	if (this->inverseMass <= 0.0F)
		return;
	assert(time > 0.0F);

//	get acceleration from forces.
	this->acceleration = this->sumForces * this->getInverseMass();
	this->acceleration += glm::vec3(0.0F, -10.0F, 0.0F);


//	update position
	this->position = (this->position + (time * this->velocity));
	if (this->position.y <= 0.0F)
		this->position.y = 0.0F;

//	update velocity
	this->velocity = powf(this->damping, time) * this->velocity;
	this->velocity = (this->velocity + (time * this->acceleration));
//	clear forces
	this->clearForces();
}

bool Particle::hasInfiniteMass()
{
	if (this->inverseMass <= 0.0F)
		return true;
	else
		return false;
}

void Particle::addForce(glm::vec3 &force)
{
	this->sumForces = sumForces + force;
}

void Particle::setPosition(const glm::vec3 &position)
{
	this->position = position;
}

glm::vec3 Particle::getPosition()
{
	return this->position;
}

void Particle::setMass(const float& mass)
{
	if (mass <= 0.0F)
	{
		this->inverseMass = 0.0F;
		return;
	}
	this->inverseMass = (1.0F / mass);
}

float Particle::getInverseMass()
{
	return this->inverseMass;
}

// !!CHECK WITH Particle::hasInfiniteMass() BEFORE CALLING!!
float Particle::getMass()
{
	return (1.0F / this->inverseMass);
}

void Particle::setDamping(const float &d)
{
	this->damping = d;
}

float Particle::getDamping()
{
	return this->damping;
}



void Particle::clearForces()
{
	this->sumForces = glm::vec3(0.0f, 0.0f, 0.0f);
}
