#ifndef WORLD_H
#define WORLD_H
#include <glad.h>
#include <vector>
#include <h/Cube.h>
#include <h/Camera.h>
#include <h/ResourceManager.h>
#include <glm/gtc/type_ptr.hpp>


enum WORLD_TYPE
{
	DEFAULT_1,
	MANY_CUBES,
	NUM_WORLD_TYPES
};

class World
{
private:
	std::vector<Cube> cubeBuffer;
	std::vector<Camera> worldCameras;
	Camera *targetCamera;
public:
	World();
	void addCamera(bool replaceTarget, const int &camWidth, const int &camHeight, const Camera &camera);
	void addCube(const glm::vec3 &color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ);
	Camera *getTargetCamera();
	std::vector<Cube> &getCubeBufferReference();
};

#endif