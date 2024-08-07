#ifndef WORLD_H
#define WORLD_H
#include <glad.h>
#include <h/Cube.h>
#include <h/Camera.h>
#include <h/ResourceManager.h>
#include <h/Particle.h>
#include <h/ForceGen.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>



enum WORLD_TYPE
{
	DEFAULT,
	GRAVITY_DEMO,
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
	void update(float time);
	void addCamera(bool replaceTarget, const int &camWidth, const int &camHeight, const Camera &camera);
	void addCube(const glm::vec3 &color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ);
	Camera *getTargetCamera();
	std::vector<Cube> &getCubeBufferReference();
};

#endif