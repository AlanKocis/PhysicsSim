#include <h/Renderer.h>

void Renderer::DrawWorld(World &world)
{
	glEnable(GL_DEPTH_TEST);
	static Engine &engineInstance = Engine::instance();
	static ResourceManager &resourceManager = engineInstance.getResourceManager();
	Camera *targetCamera = world.getTargetCamera();
	GLuint &cubeShader = resourceManager.getShader(CUBE_SHADER_ID);
	Mesh &cubeMesh = resourceManager.getMesh(CUBE_MESH_ID);
	GLuint cubeMeshVAO = cubeMesh.getVAO();

	glUseProgram(cubeShader);
	glBindVertexArray(cubeMeshVAO);

	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "proj"), 1, GL_FALSE, glm::value_ptr(targetCamera->getProjectionMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "view"), 1, GL_FALSE, glm::value_ptr(targetCamera->getViewMatrix()));


	for (Cube& cube : world.getCubeBufferReference())
	{

		glUniformMatrix4fv(glGetUniformLocation(cubeShader, "world"), 1, GL_FALSE, glm::value_ptr(cube.getWorldMatrix()));
		glDrawElements(GL_TRIANGLES, cubeMesh.getIndexBufferSize() / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	}

	glUseProgram(0);
	glBindVertexArray(0);
}

void Renderer::DrawCube(Cube& cube)
{

	static Engine& engineInstance = Engine::instance();
	if (engineInstance.getWorldRenderTarget() == nullptr)
		return;
	if (engineInstance.getWorldRenderTarget()->getTargetCamera() == nullptr)
		return;

	static ResourceManager& resourceManager = engineInstance.getResourceManager();
	GLuint& cubeShader = resourceManager.getShader(CUBE_SHADER_ID);
	Mesh& cubeMesh = resourceManager.getMesh(CUBE_MESH_ID);
	GLuint cubeMeshVAO = cubeMesh.getVAO();

	glUseProgram(cubeShader);
	glBindVertexArray(cubeMeshVAO);

	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "proj"), 1, GL_FALSE, glm::value_ptr(engineInstance.getWorldRenderTarget()->getTargetCamera()->getProjectionMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "view"), 1, GL_FALSE, glm::value_ptr(engineInstance.getWorldRenderTarget()->getTargetCamera()->getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "world"), 1, GL_FALSE, glm::value_ptr(cube.getWorldMatrix()));
	glDrawElements(GL_TRIANGLES, cubeMesh.getIndexBufferSize() / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

