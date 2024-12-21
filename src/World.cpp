#include <h/World.h>

World::World()
{
	cubeBuffer.reserve(10000);
	worldCameras.reserve(5);
	targetCamera = nullptr;
	numCubes = 0;
}

void World::update(float time)
{
	static float lol = 0;

	cubeBuffer[0].setRotation(glm::vec3(lol, 0, 0));

	for (Cube &cube : this->cubeBuffer)
	{
		cube.setRotation(glm::vec3(0, lol / 10, 0));
		cube.updatePhysicsComponent(time);
		if (cube.getTransform().pos.y <= 1.0F)
		{
			cube.addForce(glm::vec3(0, 3000000 * time, 0));
		}
	}
	


	lol += 3.0f * time;

}

void World::addCamera(bool replaceTarget, const int &camWidth, const int &camHeight, const Camera &camera)
{
	Camera& cam = worldCameras.emplace_back(camera);
	if (replaceTarget)
	{
		targetCamera = &cam;
	}
}

void World::addCube(const glm::vec3& color, const float &posX, const float &posY, const float &posZ, const float &scaleX, const float &scaleY, const float &scaleZ, const float &rotX, const float &rotY, const float &rotZ)
{
	cubeBuffer.emplace_back(Cube(color, posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ));
	this->numCubes++;
}

Camera *World::getTargetCamera()
{
	return targetCamera;
}

std::vector<Cube> &World::getCubeBufferReference()
{
	return this->cubeBuffer;
}

int World::getNumCubes()
{
	return this->numCubes;
}
