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
	PLANE_MESH_ID,
	NUM_MESHES
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

//
//

class GLmesh
{
private:
	Vertex *vertices;
	unsigned int *indices;
	GLuint vao, vbo, ebo;
	size_t vertexBufferSize;
	size_t indexBufferSize;
//
	void AllocateCubeVertices();
	//void AllocatePlaneVertices();
public:
	GLmesh();
	GLmesh(MESH_INDEX_ID ID);
	~GLmesh();
	void setVertexBufferSize(size_t size);
	void setIndexBufferSize(size_t size);
	GLuint getVAO() const;
	GLuint getEBO() const;
	void freeBuffers();
	size_t getVertexBufferSize();
	size_t getIndexBufferSize();
};

#endif