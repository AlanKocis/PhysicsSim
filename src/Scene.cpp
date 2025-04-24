#include <h/Scene.h>

Scene::Scene()
{	
	this->AllocateBuffers();
	this->FreeBuffers();
}

Scene::~Scene()
{
	this->FreeBuffers();
}

void Scene::UpdateScene(const GLFW::Window &window, float delta_time)
{
	//
	// update main camera 
	double x_offset = window.GetMouseXOffset();
	double y_offset = window.GetMouseYOffset();
	 
	double t = window.GetTime();
	if (t > 0.5)
	{
		//window.SetCursorMode(GLFW::CursorModes::Visible);

	}

	main_camera.processCameraMovement(window, delta_time);

	//main_camera.updateMousePos(500.0 * delta_time, 0.0);
	main_camera.updateMousePos(x_offset, y_offset);

	static float n = 0.0f;
	n += 0.5 * delta_time;
	uint32_t ms = t * 1000;


	static double _last_input_s = 0.0;
	double _cooldown_s = 0.1;

	if (window.KeyPressed(HOBBES_KEY_SPACE))
	{
		//cube_entities[0].physics.transform.scale = { 0.3f, 0.3f, 0.3f };
		//cube_entities[0].physics.transform.orientation = glm::normalize(glm::quat(glm::vec3(n, n, -n)));
		//cube_entities[0].physics.addForceAtBodyPoint(glm::vec3(0, 200, 0), glm::vec3(-1, 0, 0));


		if (t - _last_input_s >= _cooldown_s)
		{
			/*CubeEntity *projectile = AddCubeEntity();
	
	
			//CubeEntity *cube_projectile = AddCubeEntity();
			projectile->physics.transform.pos = main_camera.getWorldPos();
			glm::vec3 impulse = main_camera.getForwardVec();
			impulse *= 20000;

			float omega = 0.4 * sin(0.005 * t); //-.4 to 0.4
			float lambda = omega * omega * 2.0f; // positive only
			//cube_projectile->physics.transform.scale = glm::vec3(lambda);
			projectile->physics.addForceAtBodyPoint(impulse, glm::vec3(omega, -omega, 1.0f));
			_last_input_s = t;
			*/

			EntityID projectile = AddCubeEntityID();
			rigid_body_components[projectile].transform.pos = main_camera.getWorldPos();

			glm::vec3 impulse = main_camera.getForwardVec();
			impulse *= 500000000 * delta_time;
			float omega = 0.4 * sin(0.005 * t); //-.4 to 0.4
			float lambda = omega * omega * 2.0f; // positive only
			rigid_body_components[projectile].addForceAtBodyPoint(impulse, glm::vec3(omega, -omega, 1.0f));
		}
	}

	
	static double _last_reset_s = 0.0;
	double _reset_cooldown_s = 0.5;
	if (window.KeyPressed(HOBBES_KEY_ESCAPE))
	{
		if (t - _last_reset_s >= _reset_cooldown_s)
		{
			this->LoadIDTestScene();
			_last_reset_s = t;
		}
	}


//	physics
// 	update uniforms

// 

//	cube entities
	

	/*
	for (CubeEntity *&entity : cube_entities)
	{
		entity->physics.integrate(delta_time);
		entity->physics.transform.updateWorldMatrix();
		entity->physics.GenerateCubeInertiaTensors();
	}




	*/


	
	for (int i = 0; i < EntityManager::GetNumActiveEntities(); i++)
	{
		rigid_body_components[i].integrate(delta_time);
		rigid_body_components[i].transform.updateWorldMatrix();
		rigid_body_components[i].GenerateCubeInertiaTensors();
		matrix_transform_components[i] = rigid_body_components[i].transform.worldMatrix;
	}
	
	

	/*
	for (RigidBody &rb : rigid_body_components.components)
	{
		rb.integrate(delta_time);
		rb.transform.updateWorldMatrix();
		rb.GenerateCubeInertiaTensors();

		//loaded_shaders[CUBE_SHADER_ID].UseProgram();
		//loaded_shaders[CUBE_SHADER_ID].setMat4("view", main_camera.getViewMatrix());
		//loaded_shaders[CUBE_SHADER_ID].setMat4("proj", main_camera.getProjectionMatrix());
	}
	*/

	
}

CubeEntity* Scene::AddCubeEntity()
{
	CubeEntity *entity = cube_pool.AllocateChunk();
	entity = new(entity) CubeEntity();
	cube_entities.emplace_back(entity);
//		EventLogger gui call?
	num_cubes++;

	return entity;
}

EntityID Scene::AddCubeEntityID()
{
	EntityID id = EntityManager::GenEntityID();

	rigid_body_components.AddComponent(id, RigidBody());
	GLuint vao = loaded_meshes[CUBE_MESH_ID].getVAO();
	GLuint shader = loaded_shaders[CUBE_SHADER_ID].getID();
	render_components.AddComponent(id, { vao, shader, 1 });
	matrix_transform_components.AddComponent(id, glm::mat4(1.0f));

	return id;
}

void Scene::RemoveCubeEntity(CubeEntity *entity)
{
	cube_pool.FreeChunk(entity);
	std::erase(cube_entities, entity);
}

std::vector<CubeEntity*>::const_iterator Scene::GetCubeEntityBufferStartIt() const
{
	return cube_entities.begin();
}

std::vector<CubeEntity*>::const_iterator Scene::GetCubeEntityBufferEndIt() const
{
	return cube_entities.end();
}

Shader Scene::GetShader(SHADER_INDEX_ID shader_id) const
{
	return loaded_shaders[shader_id];
}

uint32_t Scene::GetEntityCount(EntityID entity_type) const
{
	return 0;
}

uint32_t Scene::GetVaoId(MESH_INDEX_ID mesh_type) const
{
	return loaded_meshes[mesh_type].getVAO();	//this is safe
}

uint32_t Scene::GetEboId(MESH_INDEX_ID mesh_type) const
{
	return loaded_meshes[mesh_type].getEBO();
}

uint32_t Scene::GetShaderId(SHADER_INDEX_ID shader_id) const
{
	return loaded_shaders[shader_id].getID();
}

void Scene::AllocateBuffers()
{
	cube_pool.Init(ENTITY_ALLOC_COUNT);
	cube_entities.clear();
	cube_entities.reserve(ENTITY_ALLOC_COUNT);

}

void Scene::FreeBuffers()
{
	//entity_physics_index_map.clear();
	//render_component_index_map.clear();
	//id_list.clear();
	cube_pool.FreeAllChunks();
	cube_entities.clear();
	num_cubes = 0;

	rigid_body_components.FreeReallocBuffers();
	render_components.FreeReallocBuffers();
	matrix_transform_components.FreeReallocBuffers();

	EntityManager::ResetIDs();

}

void Scene::LoadSceneMesh(MESH_INDEX_ID mesh_type)
{ 
//	use allocated buffer in std::vector to get pointer
//	to location to create object in memory

	GLmesh *mem_loc = &loaded_meshes[mesh_type];
	GLmesh *loaded_mesh = new(mem_loc) GLmesh(mesh_type);
	printf("success loading mesh id %d into %llu \n", (int)mesh_type, (uintptr_t)mem_loc);
}

void Scene::LoadSceneShader(SHADER_INDEX_ID shader_id)
{
	Shader &shader = loaded_shaders[shader_id];
	switch (shader_id)
	{
	case CUBE_SHADER_ID:			shader.loadShaderProgram("cubevertex.glsl", "cubefragment.glsl");			break;
	case CUBE_INSTANCED_SHADER_ID:	shader.loadShaderProgram("cubeInstancedVert.glsl", "cubefragment.glsl");		break;
	case PLANE_SHADER_ID:			break;
	}
}

void Scene::LoadAllMeshes()
{
	LoadSceneMesh(CUBE_MESH_ID);
}

void Scene::LoadAllShaders()
{
	LoadSceneShader(CUBE_SHADER_ID);
}


void Scene::LoadDefaultScene()
{

	FreeBuffers();
	LoadAllMeshes();
	LoadAllShaders();

//	psudo-plane
	CubeEntity *entity = AddCubeEntity();
	entity->physics.inverseMass = 0.0f;
	entity->physics.transform.scale = glm::vec3(25.0f, 0.0f, 25.0f);
	entity->physics.transform.pos.y = -5.0f;

	entity = AddCubeEntity();
	entity->physics.transform.pos.x = 0;
	entity->physics.transform.pos.y = 0;
	entity->physics.transform.pos.z = -3;

	entity = AddCubeEntity();
	entity->physics.transform.pos.x = 3;
	entity->physics.transform.pos.y = 5;
	entity->physics.transform.pos.z = -3;

	entity = AddCubeEntity();
	entity->physics.transform.pos.x = -3;
	entity->physics.transform.pos.y = 6;
	entity->physics.transform.pos.z = -3;

	// Seed random number generator
	srand((unsigned int)time(NULL));

	// Cube dimensions (2x2x2 centered at origin)
	const float cube_size = 100.0f;
	const float half_size = cube_size / 2.0f;

	// Y variation range
	const float y_variation = 0.1f;

	for (int i = 0; i < 9000; i++) {
		// Base position in cube (-1 to 1 range)
		float x = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float z = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float base_y = (rand() / (float)RAND_MAX) * cube_size - half_size;

		entity = AddCubeEntity();
		entity->physics.transform.scale = glm::vec3(0.5f);
		// Add slight Y variation and create vec3
		entity->physics.transform.pos = glm::vec3(
			x,
			base_y + ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * y_variation,
			z
		);
		glm::vec3 randomForce(
			std::rand() / (float)RAND_MAX * 2000,
			std::rand() % 2000,
			std::rand() % 2000);

		glm::vec3 randomBodyPoint(
			std::rand() / (float)RAND_MAX * 0.5f
		);

		entity->physics.addForceAtBodyPoint(randomForce, randomBodyPoint);
	}

}

void Scene::LoadIDTestScene()
{
	FreeBuffers();
	LoadAllMeshes();
	LoadAllShaders();

	EntityID plane = AddCubeEntityID();
	RigidBody *t = &rigid_body_components[plane];
	t->inverseMass = 0.0f;
	t->transform.scale = glm::vec3(100.0f, 0.0f, 100.0f);
	t->transform.pos.y = -5.0f;

	EntityID cube = AddCubeEntityID();
	t = &rigid_body_components[cube];
	t->transform.pos.x = -2;
	t->transform.pos.y = 3;
	t->transform.pos.z = -3;

	cube = AddCubeEntityID();
	t = &rigid_body_components[cube];
	t->transform.pos.y = 0;
	t->transform.pos.z = -1;

	cube = AddCubeEntityID();
	t = &rigid_body_components[cube];
	t->transform.pos.x = 2;
	t->transform.pos.y = 1;
	t->transform.pos.z = 3;

	// Seed random number generator
	srand((unsigned int)time(NULL));

	// Cube dimensions (2x2x2 centered at origin)
	const float cube_size = 100.0f; 
	const float half_size = cube_size / 2.0f;

	// Y variation range
	const float y_variation = 0.1f;

	float scale = 0.5f;

	for (int i = 0; i < 9000; i++) {
		// Base position in cube (-1 to 1 range)
		float x = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float z = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float base_y = (rand() / (float)RAND_MAX) * cube_size - half_size;

		cube = AddCubeEntityID();
		t = &rigid_body_components[cube];
		t->transform.scale = glm::vec3(scale);
		// Add slight Y variation and create vec3
		t->transform.pos = glm::vec3(
			x,
			base_y + ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * y_variation,
			z
		);
		glm::vec3 randomForce(
			std::rand() / (float)RAND_MAX * 2000,
			std::rand() % 2000,
			std::rand() % 2000);

		glm::vec3 randomBodyPoint(
			std::rand() / (float)RAND_MAX * scale
		);

		t->addForceAtBodyPoint(randomForce, randomBodyPoint);
	}


}
