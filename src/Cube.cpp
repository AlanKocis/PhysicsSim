#include <h/Cube.h>


Cube::Cube(const glm::vec3 &color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ)
{
	this->selected = false;
	this->colorVector = color;
	this->transform = Transform(posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ);
	this->physicsComponent.setMass(10);
	this->physicsComponent.setPosition(this->transform.pos);
	this->physicsComponent.setDamping(0.7F);
}

void Cube::updatePhysicsComponent(float time)
{
	if (!this->simulatePhysics)
		return;
	this->physicsComponent.integrate(time);
	this->transform.pos = this->physicsComponent.getPosition();
	this->transform.updateWorldMatrix();
}

glm::mat4& Cube::getWorldMatrix()
{
	return this->transform.worldMatrix;
}

bool Cube::isSelected()
{
	return this->selected;
}

void Cube::setSelected(const bool &b)
{
	this->selected = b;
}

Particle &Cube::getPhysicsComponent()
{
	return this->physicsComponent;
}

void Cube::setPos(const glm::vec3 &pos)
{
	this->transform.pos.x = pos.x;
	this->transform.pos.y = pos.y;
	this->transform.pos.z = pos.z;
	this->physicsComponent.setPosition(pos);
}

void Cube::setRotation(const glm::vec3 rot)
{
	this->transform.rotation = rot;
}

void Cube::setMass(const float & m)
{
	this->physicsComponent.setMass(m);
}

void Cube::updateWorldMatrix()
{
	this->transform.updateWorldMatrix();
}

Transform & Cube::getTransform()
{
	return this->transform;
}

void Cube::addForce(const glm::vec3 &force)
{
	this->physicsComponent.addForce(force);
}

void Cube::setPhysics(bool d)
{
	this->simulatePhysics = d;
}
