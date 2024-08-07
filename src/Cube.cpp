#include <h/Cube.h>


Cube::Cube(const glm::vec3 &color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ)
{
	this->selected = false;
	this->colorVector = color;
	this->transform = Transform(posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ);
	this->physicsComponent.setMass(10);
	this->physicsComponent.setPosition(this->transform.pos);
	this->physicsComponent.setDamping(0.99F);
}

void Cube::updatePhysicsComponent(float time)
{
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
