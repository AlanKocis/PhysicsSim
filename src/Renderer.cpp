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
		cube_shader.setMat4("world", (*entity_it)->physics.transform.worldMatrix);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		++entity_it;
	}

	
	glUseProgram(0);
	glBindVertexArray(0);
}

void GL::DrawScene_ID(Scene &scene)
{
	glClearColor(0.1F, 0.1F, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//
	//	cube entities
	Shader cube_shader = scene.GetShader(CUBE_SHADER_ID);

	int vao = scene.GetVaoId(CUBE_MESH_ID);
	//auto entity_it = scene.GetCubeEntityBufferStartIt();
	//auto stop = scene.GetCubeEntityBufferEndIt();

	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.GetEboId(CUBE_MESH_ID));
	cube_shader.UseProgram();
	cube_shader.setMat4("view", scene.main_camera.getViewMatrix());
	cube_shader.setMat4("proj", scene.main_camera.getProjectionMatrix());


	/*for (const RigidBody &rb : scene.rigid_body_components.components)
	{
		cube_shader.setMat4("world", rb.transform.worldMatrix);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	}
	*/


	for (int id = 0; id < scene.GetEntityCount(CUBE); id++)
	{
		RenderComponent &ren = scene.render_components[id];
		if (ren.should_render)
		{
			RigidBody &rb = scene.rigid_body_components[id];
			glBindVertexArray(ren.vao_id);
			cube_shader.setMat4("world", rb.transform.worldMatrix);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
	}
	
	glUseProgram(0);
	glBindVertexArray(0);
}

void GL::DrawScene_ID_instanced(Scene &scene)
{
	glClearColor(0.0F, 0.0F, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//
	//	cube entities
	Shader cube_shader = scene.GetShader(CUBE_SHADER_ID);

	int vao = scene.GetVaoId(CUBE_MESH_ID);
	//auto entity_it = scene.GetCubeEntityBufferStartIt();
	//auto stop = scene.GetCubeEntityBufferEndIt();

	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.GetEboId(CUBE_MESH_ID));
	cube_shader.UseProgram();
	cube_shader.setMat4("view", scene.main_camera.getViewMatrix());
	cube_shader.setMat4("proj", scene.main_camera.getProjectionMatrix());


	/*for (const RigidBody &rb : scene.rigid_body_components.components)
	{
		cube_shader.setMat4("world", rb.transform.worldMatrix);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	}
	*/


	int instance_count = scene.GetEntityCount(CUBE);
	if (instance_count <= 0)
		return;

	//glm::mat4 *cube_transforms = new glm::mat4[instance_count];

	GLuint matrices_vbo;
	glGenBuffers(1, &matrices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, matrices_vbo);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(glm::mat4), &scene.matrix_transform_components.components[0], GL_DYNAMIC_DRAW);


	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	//glBindVertexArray(0);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instance_count); 

	glDeleteBuffers(1, &matrices_vbo);
	/*
	//glDrawArraysInstanced()
	for (int id = 0; id < EntityManager::GetNumActiveEntities(); id++)
	{
		RenderComponent &ren = scene.render_components[id];
		if (ren.should_render)
		{
			RigidBody &rb = scene.rigid_body_components[id];
			glBindVertexArray(ren.vao_id);
			cube_shader.setMat4("world", rb.transform.worldMatrix);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
	}
	*/
	glUseProgram(0);
	glBindVertexArray(0);




}

void GL::DrawScene_ptr_instanced(Scene &scene)
{
	glClearColor(0.1F, 0.1F, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//
	//	cube entities
	Shader cube_shader = scene.GetShader(CUBE_SHADER_ID);

	int vao = scene.GetVaoId(CUBE_MESH_ID);
	//auto entity_it = scene.GetCubeEntityBufferStartIt();
	//auto stop = scene.GetCubeEntityBufferEndIt();

	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.GetEboId(CUBE_MESH_ID));
	cube_shader.UseProgram();
	cube_shader.setMat4("view", scene.main_camera.getViewMatrix());
	cube_shader.setMat4("proj", scene.main_camera.getProjectionMatrix());


	/*for (const RigidBody &rb : scene.rigid_body_components.components)
	{
		cube_shader.setMat4("world", rb.transform.worldMatrix);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	}
	*/

	int instance_count = scene.cube_entities.size();

	glm::mat4 *cube_transforms = new glm::mat4[instance_count];

	auto it = scene.cube_entities.begin();
	for (int i = 0; i < instance_count; i++)
	{
		cube_transforms[i] = (*it)->physics.transform.worldMatrix;
		it++;
	}



	GLuint matrices_vbo;
	glGenBuffers(1, &matrices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, matrices_vbo);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(glm::mat4), cube_transforms, GL_DYNAMIC_DRAW);


	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	//glBindVertexArray(0);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instance_count);

	glDeleteBuffers(1, &matrices_vbo);
	/*
	//glDrawArraysInstanced()
	for (int id = 0; id < EntityManager::GetNumActiveEntities(); id++)
	{
		RenderComponent &ren = scene.render_components[id];
		if (ren.should_render)
		{
			RigidBody &rb = scene.rigid_body_components[id];
			glBindVertexArray(ren.vao_id);
			cube_shader.setMat4("world", rb.transform.worldMatrix);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
	}
	*/
	glUseProgram(0);
	glBindVertexArray(0);

	delete []cube_transforms;
}

void GL::Viewport(int width, int height)
{
	glViewport(0, 0, width, height);
}



