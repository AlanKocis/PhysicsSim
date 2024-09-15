#include <h/Renderer.h>

void Renderer::DrawWorld(World &world)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	static Engine &engineInstance = Engine::instance();
	static ResourceManager &resourceManager = engineInstance.getResourceManager();
	Camera *targetCamera = world.getTargetCamera();
	static GLuint &cubeShader = resourceManager.getShader(CUBE_SHADER_ID);
	static GLuint &cubeOutlineShader = resourceManager.getShader(CUBE_OUTLINE_ID);
	static Mesh &cubeMesh = resourceManager.getMesh(CUBE_MESH_ID);
	static GLuint cubeMeshVAO = cubeMesh.getVAO();

	glStencilMask(0x00);


	glUseProgram(cubeShader);
	glBindVertexArray(cubeMeshVAO);


	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "proj"), 1, GL_FALSE, glm::value_ptr(targetCamera->getProjectionMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader, "view"), 1, GL_FALSE, glm::value_ptr(targetCamera->getViewMatrix()));


	for (Cube& cube : world.getCubeBufferReference())
	{
		if (cube.isSelected())
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}

		glUniformMatrix4fv(glGetUniformLocation(cubeShader, "world"), 1, GL_FALSE, glm::value_ptr(cube.getWorldMatrix()));
		glDrawElements(GL_TRIANGLES, cubeMesh.getIndexBufferSize() / sizeof(GLuint), GL_UNSIGNED_INT, 0);


		if (cube.isSelected())
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);

			Transform t = cube.getTransform();
			t.scale = glm::vec3(1.05f, 1.05f, 1.05f);
			t.updateWorldMatrix();
			
			glUseProgram(cubeOutlineShader);
			glUniformMatrix4fv(glGetUniformLocation(cubeShader, "proj"), 1, GL_FALSE, glm::value_ptr(targetCamera->getProjectionMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(cubeShader, "view"), 1, GL_FALSE, glm::value_ptr(targetCamera->getViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(cubeOutlineShader, "world"), 1, GL_FALSE, glm::value_ptr(t.worldMatrix));
			glDrawElements(GL_TRIANGLES, cubeMesh.getIndexBufferSize() / sizeof(GLuint), GL_UNSIGNED_INT, 0);

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glEnable(GL_DEPTH_TEST);
			glUseProgram(cubeShader);
		}


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

