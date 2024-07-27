#include <h/ResourceManager.h>

void ResourceManager::loadResources()
{
	this->engineMeshes[CUBE_MESH_ID] = Mesh(CUBE_MESH_ID);
	this->engineShaders[CUBE_SHADER_ID] = Shader("cubevertex.glsl", "cubefragment.glsl");
	printf("Initialized resources\n");
}

Mesh *ResourceManager::getMeshBufferPtr()
{
	return engineMeshes;
}


Shader *ResourceManager::getShaderBufferPtr()
{
	return engineShaders;
}


Mesh& ResourceManager::getMesh(const MESH_INDEX_ID &ID)
{
	return engineMeshes[ID];
}

GLuint &ResourceManager::getShader(const SHADER_INDEX_ID &ID)
{
	return (engineShaders[ID].getID());
}

ResourceManager::~ResourceManager()
{
	for (int n = 0; n < NUM_MESHES; n++)
	{
		engineMeshes[n].freeBuffers();
	}
}
