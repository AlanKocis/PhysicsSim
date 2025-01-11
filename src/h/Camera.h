#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <h/Defaults.h>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

#define DEFAULT_FOV ( glm::radians(45.0f) )
#define DEFAULT_MOUSE_SENSITIVITY 0.1f
#define DEFAULT_FLY_SPEED 10.0f
#define DEFAULT_POSITION_VECTOR {0.0f, 1.0f, 1.0f} 
#define DEFAULT_FORWARD_VECTOR {0.0f, 0.0f, -1.0f}
#define DEFAULT_ROTATION_ANGLES {0.0f, -90.0f, 0.0f}
#define DEFAULT_UP_VECTOR {0.0f, 1.0f, 0.0f}

enum CAMERA_DIRECTION
{
	FORWARD,
	LEFT,
	RIGHT,
	BACK,
	UP,
	DOWN
};

enum CAMERA_INPUT {CAMERA_TURNABLE, CAMERA_FIXED};

class Camera
{
private:
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec3 positionVector;
	vec3 forwardVector;
	vec3 upVector;
	vec3 rightVector;
	vec3 rotationAngles;

	float cameraWidth;
	float cameraHeight;
	float FOV;
	float sensitivity;
	float flySpeed;
	bool firstMouse;
	bool turnable; 
	bool moveable;
//

public:
	Camera();
	Camera(float &posX, float &posY, float &posZ, float &forwardX, float &forwardY, float &forwardZ, int &width, int &height, float &fov, float &sensitivity);
	void updateVectors();
	void updateMousePos(const double &x_offset, const double &y_offset);
	mat4 &getProjectionMatrix();
	mat4 &getViewMatrix();
	bool isTurnable();
	bool isMoveable();
	void setFirstMouse();
	void processCameraMovement(const CAMERA_DIRECTION& direction, const float& dT);
	glm::vec3 &getWorldPos();
};

#endif