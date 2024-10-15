#include <h/Transform.h>

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

	worldMatrix *= rotation;
	worldMatrix = glm::scale(worldMatrix, scale);
}
