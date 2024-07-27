#include <h/World.h>

World::World()
{
	cubeBuffer.reserve(10000);
	worldCameras.reserve(5);
	targetCamera = nullptr;
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

Camera *World::getTargetCamera() const
{
	return targetCamera;
}

void World::drawWorld(Mesh *engineMeshes, Shader *engineShaders)
{
	GLuint cubeShader = engineShaders[CUBE_SHADER_ID].getID();
	Mesh *cubeMesh = &engineMeshes[CUBE_MESH_ID];
	GLuint cubeMeshVAO = cubeMesh->getVAO();

	glUseProgram(cubeShader);
	glBindVertexArray(cubeMeshVAO);

	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "proj"), 1, GL_FALSE, glm::value_ptr(targetCamera->getProjectionMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "view"), 1, GL_FALSE, glm::value_ptr(targetCamera->getViewMatrix()));

	for (Cube &cube : cubeBuffer)
	{
		glUniformMatrix4fv(glGetUniformLocation(cubeShader, "world"), 1, GL_FALSE, glm::value_ptr(cube.getWorldMatrix()));
		glDrawElements(GL_TRIANGLES, cubeMesh->getIndexBufferSize() / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	}

	glUseProgram(0);
	glBindVertexArray(0);
}
