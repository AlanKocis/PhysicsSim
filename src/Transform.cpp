#include <h/Transform.h>

Transform::Transform()
{
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;

	orientation = glm::quat(glm::vec3(0, 0, 0));
	glm::normalize(orientation);
	updateWorldMatrix();
}

Transform::Transform(const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ)
{
	pos.x = posX;
	pos.y = posY;
	pos.z = posZ;
	
	scale.x = scaleX;
	scale.y = scaleY;
	scale.z = scaleZ;

	orientation = glm::quat(glm::vec3(rotX, rotY, rotZ));
	glm::normalize(orientation);
	updateWorldMatrix(); 
}

void Transform::updateWorldMatrix()
{
	worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 rotation = glm::mat4(orientation);

	rotation *= worldMatrix;
	worldMatrix = glm::scale(rotation, scale);
}
