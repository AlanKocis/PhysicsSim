#ifndef SCENE_H
#define SCENE_H

#include <h/Shader.h>
#include <h/Entity.h>
#include <h/Mesh.h>
#include <h/Camera.h>
#include <h/Window.h>
#include <h/MemoryPool.h>
#include <h/StaticPool.h>
#include <unordered_map>
#include <stdint.h>

struct Scene
{

//	resources
	Camera main_camera;
//
	MemoryPool<CubeEntity> cube_pool;
	std::vector<CubeEntity*> cube_entities;

	std::vector<EntityID> id_list;
	hobPool<RigidBody> physics_components;
	hobPool<RenderComponent> render_components;
	std::unordered_map<uint32_t, uint32_t> entity_physics_index_map;
	std::unordered_map<uint32_t, uint32_t> render_component_index_map;

	GLmesh loaded_meshes[MESH_INDEX_ID::NUM_MESHES];
	Shader loaded_shaders[SHADER_INDEX_ID::NUM_SHADERS];
// stores 0 for every mesh/shader type when unloaded
//
	uint32_t num_cubes;
	uint32_t num_planes;
//	update these when adding new std::vectors
	void AllocateBuffers();
	void FreeBuffers();
//	load resources depending on future reading/writing implementation
//	for now just load everything I make so far and open a default scene
	void LoadSceneMesh(MESH_INDEX_ID mesh_type);
	void LoadSceneShader(SHADER_INDEX_ID shader_id);
	void LoadAllMeshes();
	void LoadAllShaders();

	Scene();
	~Scene();
	void LoadDefaultScene();	//check this to see how to set up a LoadScene() function
	void LoadIDTestScene();
	void UpdateScene(const GLFW::Window& window, float delta_time);
	CubeEntity* AddCubeEntity();	//send an event to gui class?
	EntityID AddCubeEntityIDv();
	void RemoveCubeEntity(CubeEntity *entity);
	std::vector<CubeEntity*>::const_iterator GetCubeEntityBufferStartIt() const;
	std::vector<CubeEntity*>::const_iterator GetCubeEntityBufferEndIt() const;
	Shader GetShader(SHADER_INDEX_ID shader_id) const;
//
	uint32_t GetEntityCount(EntityID entity_type) const;
	uint32_t GetVaoId(MESH_INDEX_ID mesh_type) const;
	uint32_t GetEboId(MESH_INDEX_ID mesh_type) const;

	uint32_t GetShaderId(SHADER_INDEX_ID shader_id) const;
};

#endif