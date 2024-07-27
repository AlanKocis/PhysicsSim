#include <h/Cube.h>


Cube::Cube(const glm::vec3 &color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ)
{
	this->colorVector = color;
	this->transform = Transform(posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ);
}

glm::mat4& Cube::getWorldMatrix()
{
	return this->transform.worldMatrix;
}
