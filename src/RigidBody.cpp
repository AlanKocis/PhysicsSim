#include <h/RigidBody.h>

RigidBody::RigidBody()
{
	velocity = { 0, 0, 0 };
	angularVelocity = { 0, 0, 0 };
	acceleration = { 0, 0, 0 };
	sumForces = { 0, 0, 0 };
	sumTorques = { 0, 0, 0 };
	inverseMass = 0.5f;
	angularDamping = 0.99f;
	linearDamping = 0.99f;
	shouldRender = true;
	isAsleep = false;
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
	lastFrameAcceleration = sumForces * inverseMass;
	glm::vec3 angularAcceleration = inverseInertiaTensorWorld * sumTorques; 
	 

	lastFrameAcceleration += glm::vec3(0, -9, 0);
	//if (transform.pos.y <= -5.0f)
		//lastFrameAcceleration += glm::vec3(0, 8, 0);
	           

	velocity += lastFrameAcceleration * time;
	angularVelocity += angularAcceleration * time;
	velocity *= powf(linearDamping, time);
	angularVelocity *= powf(linearDamping, time);

	transform.pos += velocity * time;


	glm::quat q = { 0, angularVelocity.x * time * 0.5f, angularVelocity.y * time * 0.5f, angularVelocity.z * time * 0.5f };
	q *= transform.orientation;
	
	transform.orientation.x += q.x;
	transform.orientation.y += q.y;
	transform.orientation.z += q.z;
	transform.orientation.w += q.w;

	sumForces = { 0.0f, 0.0f, 0.0f };
	sumTorques = { 0.0f, 0.0f, 0.0f };

}

void RigidBody::GenerateCubeInertiaTensors()
{
	if (this->inverseMass <= 0.0F)
		return;

	//		calculate inertia tensor
	//
	float x = (transform.scale.x * transform.scale.x);
	float y = (transform.scale.y * transform.scale.y);
	float z = (transform.scale.z * transform.scale.z);



	glm::mat3 tensor;
	tensor[0][0] = (1.0f / 12.0f) * (1.0f / inverseMass) * (y + z);
	tensor[0][1] = 0;
	tensor[0][2] = 0; 
	tensor[1][0] = 0;
	tensor[1][1] = (1.0f / 12.0f) * (1.0f / inverseMass) * (x + z);
	tensor[1][2] = 0;
	tensor[2][0] = 0;
	tensor[2][1] = 0;
	tensor[2][2] = (1.0f / 12.0f) * (1.0f / inverseMass) * (x + y);
	inverseInertiaTensor = glm::inverse(tensor);
	//		Transform basis to get world coordinate inertia tensor
	//

	//inverseInertiaTensorWorld = glm::mat3(transform.worldMatrix) * inverseInertiaTensor;

	glm::mat3 R = glm::mat3_cast(transform.orientation);
	inverseInertiaTensorWorld = (R * inverseInertiaTensor * glm::transpose(R));

 
	
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
	addForceAtWorldPoint(force, transform.pos);
}

void RigidBody::addForceAtWorldPoint(const glm::vec3 &force, const glm::vec3 &point)
{
	glm::vec3 r = point - transform.pos;

	sumForces += force;
	sumTorques += glm::cross(r, force);
}

void RigidBody::addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point)
{
	glm::vec3 p = point + transform.pos;
	addForceAtWorldPoint(force, p);
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