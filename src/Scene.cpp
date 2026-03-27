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



	/*
	// Two orbiting gravity wells
	double orbit_speed = 0.5;
	float orbit_radius = 15.0f;
	glm::vec3 well1 = glm::vec3(
		orbit_radius * cos(t * orbit_speed),
		5.0f,
		orbit_radius * sin(t * orbit_speed)
	);
	glm::vec3 well2 = glm::vec3(
		orbit_radius * cos(t * orbit_speed + glm::pi<float>()),
		5.0f,
		orbit_radius * sin(t * orbit_speed + glm::pi<float>())
	);

	float well_strength = 800.0f;

	for (EntityID id : cube_entity_ids)
	{
		RigidBody& rb = rigid_body_components[id];
		if (rb.inverseMass <= 0.0f) continue; // skip static plane

		for (const glm::vec3& well : { well1, well2 })
		{
			glm::vec3 dir = well - rb.transform.pos;
			float distSq = glm::dot(dir, dir);
			distSq = glm::max(distSq, 4.0f); // clamp to avoid infinite force up close
			float forceMag = well_strength * 10 / distSq ;
			rb.addForceAtCenter(glm::normalize(dir) * forceMag);
		}
	}
	*/





	if (window.KeyPressed(HOBBES_KEY_TAB))
	{
		static double _last_toggle_s = 0.0;
		if (t - _last_toggle_s >= _cooldown_s)
		{
			_last_toggle_s = t;
			toggle_physics(); // toggle_physics now performs pause/resume once
		}
	}

	if (run_physics)
		resume_physics();
	else
		pause_physics();


	if (window.KeyPressed(HOBBES_KEY_SPACE))
	{
		//cube_entities[0].physics.transform.scale = { 0.3f, 0.3f, 0.3f };
		//cube_entities[0].physics.transform.orientation = glm::normalize(glm::quat(glm::vec3(n, n, -n)));
		//cube_entities[0].physics.addForceAtBodyPoint(glm::vec3(0, 200, 0), glm::vec3(-1, 0, 0));


		if (t - _last_input_s >= _cooldown_s)
		{
			glm::vec3 worldPos = main_camera.getWorldPos();
			Transform proj_tf = Transform(worldPos.x, worldPos.y, worldPos.z, 1, 1, 1, 0, 0, 0);
			proj_tf.scale = glm::vec3(0.5f);
			EntityID projectile = AddCubeEntityID(proj_tf);
			rigid_body_components[projectile].inverseMass = 0.2f; // make it lighter than default cubes

			glm::vec3 impulse = glm::normalize(main_camera.getForwardVec()) * 100.0f;  // was 100.0f


			// Apply linear impulse at center — no torque generated
			rigid_body_components[projectile].applyImpulseAtWorldPoint(impulse, worldPos);

			// Optional: add a small deliberate spin independently
			float spin = 2.0f;
			rigid_body_components[projectile].angularVelocity = glm::vec3(0, spin, 0);

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

			this->LoadIDTestScene();

			_last_reset_s = t;
		}
	}

	constexpr double PHYS_DT = 1.0 / 120.0;
	static double physicsAccumulator = 0.0;
	constexpr double MAX_ACCUM = 0.25;

	if (delta_time > MAX_ACCUM) delta_time = (float)MAX_ACCUM;
	physicsAccumulator += delta_time;

	if (physicsAccumulator > MAX_ACCUM) physicsAccumulator = MAX_ACCUM;

	while (physicsAccumulator >= PHYS_DT)
	{
		// --- per-entity physics + BVH maintenance ---
		for (EntityID i : cube_entity_ids)
		{
			RigidBody& rb = rigid_body_components[i];

			if (!rb.isAsleep)
				rb.integrate((float)PHYS_DT);

			rb.transform.updateWorldMatrix();
			rb.GenerateCubeInertiaTensors();
			matrix_transform_components[i] = rb.transform.worldMatrix;
		}


		BVHTree.Clear();

		// Shuffle entity IDs to balance the BVH tree
		std::vector<EntityID> shuffled_ids = cube_entity_ids;
		static std::mt19937 rng(std::random_device{}());
		std::shuffle(shuffled_ids.begin(), shuffled_ids.end(), rng);
		for (EntityID i : shuffled_ids)
		{
			RigidBody& rb = rigid_body_components[i];
			float new_radius = glm::length(rb.transform.scale) * 0.5f;
			BVHTree.AddEntity(i, BoundingSphere(rb.transform.pos, new_radius));
		}

		NarrowPhase::ProcessCollisions(*this);

		// UPDATE TRANSFORMS AFTER COLLISION RESOLUTION
		for (EntityID i : cube_entity_ids)
		{
			RigidBody& rb = rigid_body_components[i];
			rb.transform.updateWorldMatrix();
			matrix_transform_components[i] = rb.transform.worldMatrix;
		}

		physicsAccumulator -= PHYS_DT;
	}

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
	shape_data_components.AddComponent(id, ShapeData{ .type = CUBE});

	float radius = glm::max(transform.scale.x, transform.scale.y);
	radius = glm::max(radius, transform.scale.z);

	BVHTree.AddEntity(id, BoundingSphere(transform.pos, radius));

	return id;
}

EntityID Scene::AddPlaneEntityID(const Transform& transform, const ShapeData& shape_data)
{
	EntityID id = EntityManager::GenEntityID();
	cube_entity_ids.push_back(id);
	plane_entity_ids.push_back(id);

	rigid_body_components.AddComponent(id, RigidBody(transform));
	GLuint vao = loaded_meshes[CUBE_MESH_ID].getVAO();
	GLuint shader = loaded_shaders[CUBE_SHADER_ID].getID();
	render_components.AddComponent(id, { vao, shader, 1 });
	matrix_transform_components.AddComponent(id, glm::mat4(1.0f));
	shape_data_components.AddComponent(id, shape_data);

	float radius = glm::max(transform.scale.x, transform.scale.y);
	radius = glm::max(radius, transform.scale.z);

	BVHTree.AddEntity(id, BoundingSphere(transform.pos, radius));

	return id;
}

void Scene::RemoveCubeEntity(EntityID id)
{
	assert(cube_entity_ids.size() > 0);

	rigid_body_components.RemoveComponent(id);
	render_components.RemoveComponent(id);
	matrix_transform_components.RemoveComponent(id);
	//delete bsp_components[id];
	//printf("Deleted entity %I64d\n", id);
	// bsp tree requires refitting
	//bsp_components.FreeReallocBuffers();  instead of doing expensive reallocs, just use id access to reassign pointers                  			//printf("root id = %%I64d after RemoveCubeEntity()\n", bsp_root->entity_id);
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

void Scene::RemovePlaneEntity(EntityID id)
{
	assert(plane_entity_ids.size() > 0);

	rigid_body_components.RemoveComponent(id);
	render_components.RemoveComponent(id);
	matrix_transform_components.RemoveComponent(id);
	//delete bsp_components[id];
	//printf("Deleted entity %I64d\n", id);
	// bsp tree requires refitting
	//bsp_components.FreeReallocBuffers();  instead of doing expensive reallocs, just use id access to reassign pointers                  			//printf("root id = %%I64d after RemoveCubeEntity()\n", bsp_root->entity_id);
	//bsp_root->traverseRehashIDs(bsp_root, bsp_components);

	EntityManager::RecycleEntityID(id);
	BVHTree.RemoveEntity(id);

	auto it = std::find(plane_entity_ids.begin(), plane_entity_ids.end(), id);
	if (plane_entity_ids.size() > 1)
	{
		std::swap(*it, plane_entity_ids[plane_entity_ids.size() - 1]);
	}
	plane_entity_ids.pop_back();
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
	plane_entity_ids.clear();
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

void Scene::pause_physics()
{
	for (EntityID id : cube_entity_ids)
	{
		rigid_body_components[id].isAsleep = true;
	}
}

void Scene::resume_physics()
{
	for (EntityID id : cube_entity_ids)
	{
		rigid_body_components[id].isAsleep = false;
	}
}

void Scene::toggle_physics()
{
	run_physics = !run_physics;
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
	//pause_physics();

	Transform tf = Transform(0, 0, 0, 1, 1, 1, 0, 0, 0);
	tf.scale = glm::vec3(300.0f, 0.001f, 300.0f);
	tf.pos.y = -2.0f;
	PlaneData floor = { glm::vec3(0, 1, 0), 0.0f, glm::vec2(300, 300) };
	EntityID plane = AddPlaneEntityID(tf, ShapeData{ PLANE, floor });
	rigid_body_components[plane].inverseMass = 0.0f;


	srand((unsigned int)time(NULL));



	// Seed random number generator

	// Cube dimensions (2x2x2 centered at origin)
	const float cube_size = 50.0f; 
	const float half_size = cube_size / 2.0f;

	// Y variation range
	const float y_variation = 0.1f;
	 
	float scale = 1.0f;


	for (int i = 0; i < 100; i++) {
		// Base position in cube (-1 to 1 range)
		float x = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float z = (rand() / (float)RAND_MAX) * cube_size - half_size;
		float base_y = ((rand() / (float)RAND_MAX) * cube_size - half_size) + 35;

		tf = Transform(x, base_y + ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * y_variation, z, scale, scale, scale, 0, 0, 0);
		EntityID cube = AddCubeEntityID(tf);
		RigidBody *t = &rigid_body_components[cube];
		t->inverseMass = 0.02f;

		glm::vec3 randomForce(
			(std::rand() % 5000) - 1000,
			(std::rand() % 5000) - 1000,
			(std::rand() % 5000) - 1000
		);

		glm::vec3 randomBodyPoint(
			(std::rand() / (float)RAND_MAX) - 0.5f,
			(std::rand() / (float)RAND_MAX) - 0.5f,
			(std::rand() / (float)RAND_MAX) - 0.5f
		);

		//t->addForceAtBodyPoint(-(t->getPosition() * 100.0f * (1/glm::length(t->getPosition()))), randomBodyPoint);
	}

}
