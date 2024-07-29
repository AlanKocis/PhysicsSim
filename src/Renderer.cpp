#include <h/Renderer.h>

void Renderer::DrawWorld(World &world)
{
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