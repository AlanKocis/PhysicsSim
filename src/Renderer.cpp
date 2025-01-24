#include <h/Renderer.h>


/*
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


	for (Cube cube : world.getCubeBufferReference())
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

*/

void GL::InitRenderer()
{
	glEnable(GL_DEPTH_TEST);
}

void GL::StartRenderPass()
{
}

void GL::DrawScene(const Scene& scene)
{
	glClearColor(0.1F, 0.1F, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//	cube entities
	Shader cube_shader = scene.GetShader(CUBE_SHADER_ID);

	int vao = scene.GetVaoId(CUBE_MESH_ID);
	auto entity_it = scene.GetCubeEntityBufferStartIt();
	auto stop = scene.GetCubeEntityBufferEndIt();

	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.GetEboId(CUBE_MESH_ID));
	cube_shader.UseProgram();

	while (entity_it != stop)
	{
		cube_shader.setMat4("world", entity_it->physics.transform.worldMatrix);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		++entity_it;
	}

	
	glUseProgram(0);
	glBindVertexArray(0);
}

void GL::Viewport(int width, int height)
{
	glViewport(0, 0, width, height);
}
