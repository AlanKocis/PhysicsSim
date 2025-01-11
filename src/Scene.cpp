#include <h/Scene.h>

Scene::Scene()
{	
	this->FreeAllocateBuffers();
}

Scene::~Scene()
{
	this->FreeBuffers();
}

void Scene::UpdateScene(const double &x_offset, const double &y_offset, float delta_time)
{
	//
	// update main camera
	main_camera.updateMousePos(x_offset, y_offset);
	 

	//	physics
	//	update uniforms
	//	cube entities


	for (auto ce_it = cube_entities.begin(); ce_it != cube_entities.end(); ce_it++)
	{
		ce_it->physics.integrate(delta_time);
		ce_it->physics.transform.updateWorldMatrix();
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

	cube_entities.reserve(1000);
	plane_entities.reserve(1000);

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
	printf("success loading mesh id %d into %ld \n", (int)mesh_type, mem_loc);
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

	AddEntity(CUBE);
	cube_entities[num_cubes - 1].physics.transform.pos.x = 0;
	cube_entities[num_cubes - 1].physics.transform.pos.y = 0;
	cube_entities[num_cubes - 1].physics.transform.pos.z = -3;

	AddEntity(CUBE);
	cube_entities[num_cubes - 1].physics.transform.pos.x = 3;
	cube_entities[num_cubes - 1].physics.transform.pos.y = 0;
	cube_entities[num_cubes - 1].physics.transform.pos.z = -3;

	AddEntity(CUBE);
	cube_entities[num_cubes - 1].physics.transform.pos.x = -3;
	cube_entities[num_cubes - 1].physics.transform.pos.y = 0;
	cube_entities[num_cubes - 1].physics.transform.pos.z = -3;

}