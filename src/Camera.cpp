#include <h/Camera.h>

Camera::Camera()
{
//	from Hobbes.h
	cameraWidth = DEFAULT_WINDOW_WIDTH;
	cameraHeight = DEFAULT_WINDOW_HEIGHT;
//
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
	/*
	forwardVector.x = cos(glm::radians(rotationAngles.y)) * cos(glm::radians(rotationAngles.x));
	forwardVector.y = sin(glm::radians(rotationAngles.x));
	forwardVector.z = sin(glm::radians(rotationAngles.y)) * cos(glm::radians(rotationAngles.x));
	forwardVector = glm::normalize(forwardVector);
	rightVector = glm::normalize(glm::cross(forwardVector, DEFAULT_UP_VECTOR));
	upVector = glm::cross(rightVector, forwardVector);
	viewMatrix = glm::lookAt(positionVector, positionVector + forwardVector, DEFAULT_UP_VECTOR);
	*/

	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(rotationAngles.y)) * cos(glm::radians(rotationAngles.x));
	front.y = sin(glm::radians(rotationAngles.x));
	front.z = sin(glm::radians(rotationAngles.y)) * cos(glm::radians(rotationAngles.x));
	forwardVector = glm::normalize(front);
	// also re-calculate the Right and Up vector
	rightVector = glm::normalize(glm::cross(forwardVector, DEFAULT_UP_VECTOR));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	upVector = glm::normalize(glm::cross(rightVector, forwardVector));
	viewMatrix = glm::lookAt(positionVector, positionVector + forwardVector, DEFAULT_UP_VECTOR);
	this->projectionMatrix = glm::perspective(FOV, (cameraWidth / cameraHeight), 0.1f, 100.0f);

}

void Camera::updateMousePos(const double& x_offset, const double& y_offset)
{
	if (!this->isTurnable())
		return;

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

void Camera::setWidth(float width)
{
	this->cameraWidth = width;
}

void Camera::setHeight(float height)
{
	this->cameraHeight = height;
}



void Camera::processCameraMovement(const GLFW::Window &window, const float &dT)
{
	if (!this->isMoveable())
		return;

	glm::vec3 move_dir{0, 0, 0};

	if (window.KeyPressed(HOBBES_KEY_W))
		move_dir += this->forwardVector;
	if (window.KeyPressed(HOBBES_KEY_S))
		move_dir -= this->forwardVector;
	if (window.KeyPressed(HOBBES_KEY_A))
		move_dir -= this->rightVector;
	if (window.KeyPressed(HOBBES_KEY_D))
		move_dir += this->rightVector;
	if (window.KeyPressed(HOBBES_KEY_Q))
		move_dir.y -= 1;
	if (window.KeyPressed(HOBBES_KEY_E))
		move_dir.y += 1;

	move_dir /= move_dir.length();

	float SPEED = flySpeed * dT;
	move_dir *= SPEED;

	this->positionVector += move_dir;
	/*switch (direction)
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
	//if (this->positionVector.y < 0.0F)
		//this->positionVector.y = 0.0F;
		*/
}

glm::vec3 &Camera::getWorldPos()
{
	return this->positionVector;
}
