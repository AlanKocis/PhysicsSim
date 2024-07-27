#ifndef MESH_H
#define MESH_H
#include <glad.h>
#include <vector>
#include <stdint.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

enum MESH_INDEX_ID
{
	CUBE_MESH_ID,
	NUM_MESHES
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh
{
private:
	GLuint vao, vbo, ebo;
	Vertex *vertices;
	GLuint *indices;
	size_t vertexBufferSize, indexBufferSize;
public:
	Mesh();
	Mesh(MESH_INDEX_ID ID);
	void setVertexBufferSize(size_t size);
	void setIndexBufferSize(size_t size);
	GLuint getVAO();
	void freeBuffers();
	size_t getVertexBufferSize();
	size_t getIndexBufferSize();
};

void copyCubeMeshData(Mesh *mesh, Vertex *vertices, GLuint *indices);
#endif