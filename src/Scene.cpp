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
	double _cooldown_s = 0.4;

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
			glm::vec3 worldPos = main_camera.getWorldPos();
			Transform proj_tf = Transform(worldPos.x, worldPos.y, worldPos.z, 1, 1, 1, 0, 0, 0);
			EntityID projectile = AddCubeEntityID(proj_tf);

			glm::vec3 impulse = main_camera.getForwardVec();
			int len = impulse.length();
			impulse /= len;
			impulse *= (50000);
			float omega = 0.1 * sin(0.005 * t); //-.4 to 0.4
			rigid_body_components[projectile].addForceAtBodyPoint(impulse, glm::vec3(omega, -omega, 1.0f));
			_last_input_s = t;
		}
	}

	



	static double _last_reset_s = 0.0;
	double _reset_cooldown_s = 0.5;
	if (window.KeyPressed(HOBBES_KEY_ESCAPE))
	{
		if (t - _last_reset_s >= _reset_cooldown_s)
		{
			//this->LoadIDTestScene();
			while (!cube_entity_ids.empty())
			{
				RemoveCubeEntity(cube_entity_ids.front());
			}


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


	
	for (EntityID i : cube_entity_ids)
	{
		glm::vec3 pos_naught = rigid_body_components[i].transform.pos;

		rigid_body_components[i].integrate(delta_time);
		rigid_body_components[i].transform.updateWorldMatrix();
		rigid_body_components[i].GenerateCubeInertiaTensors();		// not sure, but i think this doesnt have to be done each frame, update once when changing dimensions?
		matrix_transform_components[i] = rigid_body_components[i].transform.worldMatrix;

		glm::vec3 pos_diff = rigid_body_components[i].transform.pos - pos_naught;
		if (BVHTree.id_lookup.Exists(i))
		{
			BoundingSphere other = BVHTree.id_lookup[i]->volume;
			float new_radius = glm::length(rigid_body_components[i].transform.scale) * 0.5f;
			if (glm::length(pos_diff) > other.radius)
			{
				BVHTree.RemoveEntity(i);
				BVHTree.AddEntity(i, BoundingSphere(rigid_body_components[i].transform.pos, new_radius));
			}
		}
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


EntityID Scene::AddCubeEntityID(const Transform &transform)
{
	EntityID id = EntityManager::GenEntityID();
	cube_entity_ids.push_back(id);

	rigid_body_components.AddComponent(id, RigidBody(transform));
	GLuint vao = loaded_meshes[CUBE_MESH_ID].getVAO();
	GLuint shader = loaded_shaders[CUBE_SHADER_ID].getID();
	render_components.AddComponent(id, { vao, shader, 1 });
	matrix_transform_components.AddComponent(id, glm::mat4(1.0f));

	float radius = glm::max(transform.scale.x, transform.scale.y);
	radius = glm::max(radius, transform.scale.z);

	BVHTree.AddEntity(id, BoundingSphere(transform.pos, radius));

	/*BVHNode<BoundingSphere> *bsp_ptr;
	if (!bsp_root)
	{
		bsp_root = new BVHNode<BoundingSphere>(NULL, BoundingSphere(transform.pos, radius), id);
		bsp_ptr = bsp_root;
	}
	else
	{
		bsp_ptr = bsp_root->insert(id, BoundingSphere(transform.pos, radius));
	}
	bsp_components.AddComponent(id, bsp_ptr);

	printf("root id after AddCubeEntity(): %I64d\n", id);
	*/


	return id;
}

void Scene::RemoveCubeEntity(EntityID id)
{
	assert(cube_entity_ids.size() > 0);

	rigid_body_components.RemoveComponent(id);
	render_components.RemoveComponent(id);
	matrix_transform_components.RemoveComponent(id);
	//delete bsp_components[id];
	printf("Deleted entity %I64d\n", id);
	// bsp tree requires refitting
	//bsp_components.FreeReallocBuffers();  instead of doing expensive reallocs, just use id access to reassign pointers                  
	//printf("root id = %%I64d after RemoveCubeEntity()\n", bsp_root->entity_id);
	//bsp_root->traverseRehashIDs(bsp_root, bsp_components);

	EntityManager::RecycleEntityID(id);
	BVHTree.RemoveEntity(id);

	auto it = std::find(cube_entity_ids.begin(), cube_entity_ids.end(), id);
	if (cube_entity_ids.size() > 1)
	{
		std::swap(*it, cube_entity_ids[cube_entity_ids.size() - 1]);
	}
	cube_entity_ids.pop_back();


}

void Scene::RemoveCubeEntity(CubeEntity *entity)
{
	//cube_pool.FreeChunk(entity);
	//std::erase(cube_entities, entity);
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
	switch (entity_type)
	{
	case CUBE:
		return cube_entity_ids.size();
	}
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
	cube_entity_ids.reserve(ENTITY_ALLOC_COUNT);
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
	cube_entity_ids.clear();
	EntityManager::ResetIDs();

	//delete bsp_root;
	//bsp_root = nullptr;
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
	/*
	FreeBuffers();
	LoadAllMeshes();
	LoadAllShaders();

//	psudo-plane
	//CubeEntity *entity = AddCubeEntity();
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
			0, 1, 0
		);

		glm::vec3 randomBodyPoint(
			(std::rand() / (float)RAND_MAX)

		);

		entity->physics.addForceAtBodyPoint(randomForce, randomBodyPoint);
	}
	*/

}

void Scene::LoadIDTestScene()
{
	FreeBuffers();
	LoadAllMeshes();
	LoadAllShaders();


	Transform tf = Transform(0, 0, 0, 1, 1, 1, 0, 0, 0);
	EntityID plane = AddCubeEntityID(tf);
	RigidBody *t = &rigid_body_components[plane];
	t->inverseMass = 0.0f;
	t->transform.scale = glm::vec3(100.0f, 0.0f, 100.0f);
	t->transform.pos.y = -5.0f;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < 5; i++)
	{
		tf.pos.x = (rand() % 21) - 20;
		tf.pos.y = (rand() % 11) - 10 + 10;
		tf.pos.z = (rand() % 21) - 20;

		plane = AddCubeEntityID(tf);

	}

//	printf("root id = %I64d after loadIdTestScene()\n", bsp_root->entity_id);







/*
	// Seed random number generator

	// Cube dimensions (2x2x2 centered at origin)
	const float cube_size = 5.0f; 
	const float half_size = cube_size / 2.0f;

	// Y variation range
	const float y_variation = 0.1f;
	 
	float scale = 1.0f;


	for (int i = 0; i < 9000; i++) {
		// Base position in cube (-1 to 1 range)
		float x = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float z = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float base_y = ((rand() / (float)RAND_MAX) * cube_size - half_size) + 35;

		tf = Transform(x, base_y + ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * y_variation, z, scale, scale, scale, 0, 0, 0);
		cube = AddCubeEntityID(tf);
		t = &rigid_body_components[cube];

		glm::vec3 randomForce(
			(std::rand() % 2000) - 1000,
			(std::rand() % 2000) - 1000,
			(std::rand() % 2000) - 1000
		);

		glm::vec3 randomBodyPoint(
			(std::rand() / (float)RAND_MAX) - 0.5f,
			(std::rand() / (float)RAND_MAX) - 0.5f,
			(std::rand() / (float)RAND_MAX) - 0.5f
		);

		t->addForceAtBodyPoint(randomForce, randomBodyPoint);
	}
*/

}
