#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

#define DEFAULT_FOV ( glm::radians(45.0F) )
#define DEFAULT_MOUSE_SENSITIVITY 0.1F
#define DEFAULT_FLY_SPEED 0.005F
#define DEFAULT_POSITION_VECTOR {0.0F, 1.0F, 1.0F} 
#define DEFAULT_FORWARD_VECTOR {0.0F, 0.0F, -1.0F}
#define DEFAULT_ROTATION_ANGLES {0.0F, -90.0F, 0.0F}
#define DEFAULT_UP_VECTOR {0.0F, 1.0F, 0.0F}

enum CAMERA_DIRECTION
{
	FORWARD,
	LEFT,
	RIGHT,
	BACK,
	UP,
	DOWN
};

class Camera
{
private:
	vec3 positionVector;
	vec3 forwardVector;
	vec3 upVector;
	vec3 rightVector;
	vec3 rotationAngles;
	mat4 projectionMatrix;
	mat4 viewMatrix;
	float cameraWidth, cameraHeight, FOV, sensitivity, flySpeed;
	bool firstMouse, turnable, moveable;
public:
	Camera(int& width, int& height);
	Camera(float &posX, float &posY, float &posZ, float &forwardX, float &forwardY, float &forwardZ, int &width, int &height, float &fov, float &sensitivity);
	void updateVectors();
	void updateMousePos(const double &xPos, const double &yPos, double lastXPos, double lastYPos);
	mat4 &getProjectionMatrix();
	mat4 &getViewMatrix();
	bool isTurnable();
	bool isMoveable();
	void processCameraMovement(const CAMERA_DIRECTION& direction);
};


#endif