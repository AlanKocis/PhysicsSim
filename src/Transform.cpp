#include <h/Transform.h>

Transform::Transform(const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ)
{
	pos.x = posX;
	pos.y = posY;
	pos.z = posZ;
	
	scale.x = scaleX;
	scale.y = scaleY;
	scale.z = scaleZ;

	rotation.x = rotX;
	rotation.y = rotY;
	rotation.z = rotZ;
	updateWorldMatrix();
}

void Transform::updateWorldMatrix()
{
	worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(pos.x, pos.y, pos.z));
	worldMatrix = glm::rotate(worldMatrix, rotation.x, glm::vec3(1, 0, 0));
	worldMatrix = glm::rotate(worldMatrix, rotation.y, glm::vec3(0, 1, 0));
	worldMatrix = glm::rotate(worldMatrix, rotation.z, glm::vec3(0, 0, 1));
	worldMatrix = glm::scale(worldMatrix, scale);
}
