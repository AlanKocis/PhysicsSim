#include <h/Camera.h>

Camera::Camera(int &width, int &height)
{
	cameraWidth = width;
	cameraHeight = height;
	positionVector = DEFAULT_POSITION_VECTOR;
	forwardVector = DEFAULT_FORWARD_VECTOR;
	rotationAngles = DEFAULT_ROTATION_ANGLES;
	FOV = DEFAULT_FOV;
	flySpeed = DEFAULT_FLY_SPEED;
	sensitivity = DEFAULT_MOUSE_SENSITIVITY;
	firstMouse = true;
	turnable = true;
	moveable = true;
	projectionMatrix = glm::perspective(FOV, (cameraWidth / cameraHeight), 0.1f, 100.0f);
	updateVectors();
}


Camera::Camera(float &posX, float &posY, float &posZ, float &forwardX, float &forwardY, float &forwardZ, int &width, int &height, float &fov, float &sensitivity)
{
	this->positionVector = { posX, posY, posZ };
	this->forwardVector = { forwardX, forwardY, forwardZ };
	this->rotationAngles = DEFAULT_ROTATION_ANGLES;
	this->cameraWidth = width;
	this->cameraHeight = height;
	this->FOV = fov;
	this->sensitivity = sensitivity;
	this->firstMouse = true;
	this->projectionMatrix = glm::perspective(FOV, (cameraWidth / cameraHeight), 0.1f, 100.0f);
	this->turnable = true;
	this->moveable = true;
	this->flySpeed = DEFAULT_FLY_SPEED;

	this->updateVectors();
}

void Camera::updateVectors()
{
	forwardVector.x = cos(glm::radians(rotationAngles.y)) * cos(glm::radians(rotationAngles.x));
	forwardVector.y = sin(glm::radians(rotationAngles.x));
	forwardVector.z = sin(glm::radians(rotationAngles.y)) * cos(glm::radians(rotationAngles.x));
	forwardVector = glm::normalize(forwardVector);
	rightVector = glm::normalize(glm::cross(DEFAULT_UP_VECTOR, -forwardVector));
	upVector = glm::cross(-forwardVector, rightVector);
	viewMatrix = glm::lookAt(positionVector, positionVector + forwardVector, DEFAULT_UP_VECTOR);
}

void Camera::updateMousePos(const double& xPos, const double& yPos, double lastXPos, double lastYPos)
{
	if (!this->isTurnable())
		return;
	if (firstMouse)
	{
		lastXPos = xPos;
		lastYPos = yPos;
		firstMouse = false;
	}

	float x_offset = xPos - lastXPos;
	float y_offset = lastYPos - yPos;

	rotationAngles.y += x_offset * sensitivity;
	rotationAngles.x += y_offset * sensitivity;

	if (rotationAngles.x > 89.0f)
		rotationAngles.x = 89.0f;
	if (rotationAngles.x < -89.0f)
		rotationAngles.x = -89.0f;

	updateVectors();
}

mat4 &Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

mat4 &Camera::getViewMatrix()
{
	return this->viewMatrix;
}

bool Camera::isTurnable()
{
	return turnable;
}

bool Camera::isMoveable()
{
	return moveable;
}

void Camera::setFirstMouse()
{
	this->firstMouse = true;
}



void Camera::processCameraMovement(const CAMERA_DIRECTION &direction, const float& dT)
{
	if (!this->isMoveable())
		return;


	float SPEED = flySpeed * dT;
	switch (direction)
	{
	case FORWARD:
		this->positionVector += SPEED * this->forwardVector;
		break;
	case BACK:
		this->positionVector -= SPEED * this->forwardVector;
		break;
	case LEFT:
		this->positionVector -= SPEED * this->rightVector;
		break;
	case RIGHT:
		this->positionVector += SPEED * this->rightVector;
		break;
	case UP:
		this->positionVector.y += SPEED;
		break;
	case DOWN:
		this->positionVector.y -= SPEED;
		break;
	}

	this->updateVectors();

}
