#include <h/World.h>

World::World()
{
	cubeBuffer.reserve(10000);
	worldCameras.reserve(5);
	targetCamera = nullptr;
}

void World::update(float time)
{
	for (Cube &cube : this->cubeBuffer)
	{
		cube.updatePhysicsComponent(time);
	}



}

void World::addCamera(bool replaceTarget, const int &camWidth, const int &camHeight, const Camera &camera)
{
	Camera& cam = worldCameras.emplace_back(camera);
	if (replaceTarget)
		targetCamera = &cam;
}

void World::addCube(const glm::vec3& color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ)
{
	cubeBuffer.emplace_back(Cube(color, posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ));

}

Camera *World::getTargetCamera()
{
	return targetCamera;
}

std::vector<Cube> &World::getCubeBufferReference()
{
	return this->cubeBuffer;
}
