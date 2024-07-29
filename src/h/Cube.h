#ifndef CUBE_H
#define CUBE_H

#include <h/Mesh.h>
#include <glm/vec3.hpp>
#include <h/Transform.h>
#include <h/Camera.h>

class Cube
{
private:
	bool selected;
	glm::vec3 colorVector;
	Transform transform;
	//Physics physics;
public:
	Cube(const glm::vec3 &color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ);
	glm::mat4& getWorldMatrix();
	bool isSelected();
};
#endif