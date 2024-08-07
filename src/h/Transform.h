#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::mat4 worldMatrix;
	Transform() = default;
	Transform(const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ);
	void updateWorldMatrix();
};


#endif