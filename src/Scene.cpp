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
		main_camera.updateMousePos(x_offset, y_offset);

	}

	static float n = 0.0f;
	n += 0.5 * delta_time;
	uint32_t ms = t * 1000;

	main_camera.processCameraMovement(window, delta_time);
	


	
	static double _last_input_s = 0.0;
	double _cooldown_s = 0.05;

	if (window.KeyPressed(HOBBES_KEY_SPACE))
	{
		//cube_entities[0].physics.transform.scale = { 0.3f, 0.3f, 0.3f };
		//cube_entities[0].physics.transform.orientation = glm::normalize(glm::quat(glm::vec3(n, n, -n)));
		//cube_entities[0].physics.addForceAtBodyPoint(glm::vec3(0, 200, 0), glm::vec3(-1, 0, 0));


		if (t - _last_input_s >= _cooldown_s)
		{
			EntityID projectile = AddCubeEntityIDv();
			int p_index = entity_physics_index_map[projectile];
			RigidBody *projectile_rb = &physics_components.buffer[p_index];
			//CubeEntity *cube_projectile = AddCubeEntity();
			projectile_rb->transform.pos = main_camera.getWorldPos();
			glm::vec3 impulse = main_camera.getForwardVec();
			impulse *= 20000;

			float omega = 0.4 * sin(0.005 * t); //-.4 to 0.4
			float lambda = omega * omega * 2.0f; // positive only
			//cube_projectile->physics.transform.scale = glm::vec3(lambda);
			projectile_rb->addForceAtBodyPoint(impulse, glm::vec3(omega, -omega, 1.0f));
			_last_input_s = t;
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
// 
// 


//	cube entities
	/*
	for (CubeEntity *&entity : cube_entities)
	{
		entity->physics.integrate(delta_time);
		entity->physics.transform.updateWorldMatrix();
		entity->physics.GenerateCubeInertiaTensors();
		loaded_shaders[CUBE_SHADER_ID].UseProgram();
		loaded_shaders[CUBE_SHADER_ID].setMat4("view", main_camera.getViewMatrix());
		loaded_shaders[CUBE_SHADER_ID].setMat4("proj", main_camera.getProjectionMatrix());
	}*/

	for (int i = 0; i < ENTITY_ALLOC_COUNT; i++)
	{
		RigidBody *rb = &physics_components.buffer[i];
		rb->integrate(delta_time);
		rb->transform.updateWorldMatrix();
		rb->GenerateCubeInertiaTensors();
		loaded_shaders[CUBE_SHADER_ID].UseProgram();
		loaded_shaders[CUBE_SHADER_ID].setMat4("view", main_camera.getViewMatrix());
		loaded_shaders[CUBE_SHADER_ID].setMat4("proj", main_camera.getProjectionMatrix());
	}


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

EntityID Scene::AddCubeEntityIDv()
{
	EntityID id = EntityManager::GenEntityID();
	int physics_index = physics_components.alloc();
	int render_index = render_components.alloc();

	entity_physics_index_map[id] = physics_index;
	render_component_index_map[id] = render_index;

	RigidBody* rb = new(&physics_components.buffer[physics_index]) RigidBody();
	RenderComponent* rc = new(&render_components.buffer[render_index]) RenderComponent();

	rc->vao_id = loaded_meshes[CUBE_MESH_ID].getVAO();
	rc->shader_id = loaded_shaders[CUBE_SHADER_ID].getID();
	rc->should_render = 1;
	id_list.emplace_back(id);
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
	cube_pool.Init(2000);
	cube_entities.clear();
	cube_entities.reserve(2000);

}

void Scene::FreeBuffers()
{
	entity_physics_index_map.clear();
	render_component_index_map.clear();
	id_list.clear();
	cube_pool.FreeAllChunks();
	cube_entities.clear();
	num_cubes = 0;

	physics_components.free_all();
	render_components.free_all();

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
	case CUBE_SHADER_ID:	shader.loadShaderProgram("cubevertex.glsl", "cubefragment.glsl");	break;
	case PLANE_SHADER_ID:	break;
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


}

void Scene::LoadIDTestScene()
{
	FreeBuffers();
	LoadAllMeshes();
	LoadAllShaders();

	entity_physics_index_map.reserve(ENTITY_ALLOC_COUNT);
	entity_physics_index_map.reserve(ENTITY_ALLOC_COUNT);
	id_list.reserve(ENTITY_ALLOC_COUNT);

	EntityID plane = AddCubeEntityIDv();
	int p_index = entity_physics_index_map[plane];
	RigidBody *t = &physics_components.buffer[p_index];
	t->inverseMass = 0.0f;
	t->transform.scale = glm::vec3(25.0f, 0.0f, 25.0f);
	t->transform.pos.y = -5.0f;



	EntityID cube = AddCubeEntityIDv();
	p_index = entity_physics_index_map[cube];

	t = &physics_components.buffer[p_index];
	t->transform.pos.x = -2;
	t->transform.pos.y = 3;
	t->transform.pos.z = -3;

	cube = AddCubeEntityIDv();
	p_index = entity_physics_index_map[cube];

	t = &physics_components.buffer[p_index];
	t->transform.pos.x = 1;
	t->transform.pos.y = 0;
	t->transform.pos.z = -1;

	cube = AddCubeEntityIDv();
	p_index = entity_physics_index_map[cube];

	t = &physics_components.buffer[p_index];
	t->transform.pos.x = 2;
	t->transform.pos.y = 1;
	t->transform.pos.z = 3;
}
