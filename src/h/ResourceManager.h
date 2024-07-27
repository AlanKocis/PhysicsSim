#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <h/Mesh.h>
#include <h/Shader.h>

class ResourceManager
{
private:
	Mesh engineMeshes[MESH_INDEX_ID::NUM_MESHES];
	Shader engineShaders[SHADER_INDEX_ID::NUM_SHADERS];
public:
	ResourceManager() = default;
	void loadResources();
	Mesh *getMeshBufferPtr();
	Shader *getShaderBufferPtr();
	Mesh &getMesh(const MESH_INDEX_ID &ID);
	GLuint &getShader(const SHADER_INDEX_ID &ID);
	~ResourceManager();
};


#endif