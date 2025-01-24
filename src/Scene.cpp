#include <h/Scene.h>

Scene::Scene()
{	
	this->FreeAllocateBuffers();
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
	double _cooldown_s = 0.1;

	if (window.KeyPressed(HOBBES_KEY_SPACE))
	{
		//cube_entities[0].physics.transform.scale = { 0.3f, 0.3f, 0.3f };
		//cube_entities[0].physics.transform.orientation = glm::normalize(glm::quat(glm::vec3(n, n, -n)));
		//cube_entities[0].physics.addForceAtBodyPoint(glm::vec3(0, 200, 0), glm::vec3(-1, 0, 0));


		if (t - _last_input_s >= _cooldown_s)
		{
			AddEntity(CUBE);
			cube_entities[num_cubes - 1].physics.transform.pos = main_camera.getWorldPos();
			glm::vec3 impulse{ 0, 0, 0 };
			impulse += main_camera.getForwardVec();
			impulse /= impulse.length();

			impulse *= 200000;

			float omega = 0.4 * sin(5 * t); //-.4 to 0.4
			cube_entities[num_cubes - 1].physics.transform.scale = glm::vec3(omega * 2.0f);

			float lambda = omega * omega * 2.0f; // positive only
			cube_entities[num_cubes - 1].physics.addForceAtBodyPoint(impulse, glm::vec3(-lambda, omega*2.0f, omega*2.0f));
			_last_input_s = t;
		}
	}

	static double _last_reset_s = 0.0;
	double _reset_cooldown_s = 0.5;
	if (window.KeyPressed(HOBBES_KEY_ESCAPE))
	{
		if (t - _last_reset_s >= _reset_cooldown_s)
		{
			this->LoadDefaultScene();
			_last_reset_s = t;
		}
	}


















//	physics
// 	update uniforms

// 
//	cube entities
	for (auto entity = cube_entities.begin(); entity != cube_entities.end(); entity++)
	{
		entity->physics.integrate(delta_time);
		entity->physics.transform.updateWorldMatrix();
		entity->physics.GenerateCubeInertiaTensors();
		loaded_shaders[CUBE_SHADER_ID].UseProgram();
		loaded_shaders[CUBE_SHADER_ID].setMat4("view", main_camera.getViewMatrix());
		loaded_shaders[CUBE_SHADER_ID].setMat4("proj", main_camera.getProjectionMatrix());
	}

}

void Scene::AddEntity(EntityID entity_type)
{
	switch (entity_type)
	{
	case CUBE:
		cube_entities.emplace_back(CubeEntity());
		cube_entities[num_cubes].vaoID = loaded_meshes[CUBE_MESH_ID].getVAO();
		cube_entities[num_cubes].shaderID = loaded_shaders[CUBE_SHADER_ID].getID();
//		EventLogger gui call?
		num_cubes++;
		break;	
//
//
	case PLANE:
		plane_entities.emplace_back(PlaneEntity());
		num_planes++;
		break;
	}
}

std::vector<CubeEntity>::const_iterator Scene::GetCubeEntityBufferStartIt() const
{
	return cube_entities.begin();
}

std::vector<CubeEntity>::const_iterator Scene::GetCubeEntityBufferEndIt() const
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
	case CUBE:	return num_cubes;	break;
	case PLANE:	return num_planes;	break;
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

void Scene::FreeAllocateBuffers()
{
	FreeBuffers();

	cube_entities.reserve(2000);
	plane_entities.reserve(2000);

	num_cubes = 0;
	num_planes = 0;
}

void Scene::FreeBuffers()
{
	cube_entities.clear();
	plane_entities.clear();
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

	FreeAllocateBuffers();
	LoadAllMeshes();
	LoadAllShaders();

//	psudo-plane
	AddEntity(CUBE);
	cube_entities[num_cubes - 1].physics.inverseMass = 0.0f;
	cube_entities[num_cubes - 1].physics.transform.scale = glm::vec3(25.0f, 0.0f, 25.0f);
	cube_entities[num_cubes - 1].physics.transform.pos.y = -20.0f;

	AddEntity(CUBE);
	cube_entities[num_cubes - 1].physics.transform.pos.x = 0;
	cube_entities[num_cubes - 1].physics.transform.pos.y = 0;
	cube_entities[num_cubes - 1].physics.transform.pos.z = -3;

	AddEntity(CUBE);
	cube_entities[num_cubes - 1].physics.transform.pos.x = 3;
	cube_entities[num_cubes - 1].physics.transform.pos.y = 5;
	cube_entities[num_cubes - 1].physics.transform.pos.z = -3;

	AddEntity(CUBE);
	cube_entities[num_cubes - 1].physics.transform.pos.x = -3;
	cube_entities[num_cubes - 1].physics.transform.pos.y = 6;
	cube_entities[num_cubes - 1].physics.transform.pos.z = -3;


}