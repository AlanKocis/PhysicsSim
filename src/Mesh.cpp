#include <h/Mesh.h>

void GLmesh::AllocateCubeVertices()
{
	float x = 1.0f, y = 1.0f, z = 1.0f;
	Vertex cubeVertices[24] = {
		//front face
		//pos			normal				tex
		{glm::vec3(-x / 2.0f, -y / 2.0f, z / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2.0f, -y / 2.0f, z / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},

		//back face
		{glm::vec3(-x / 2.0f, -y / 2.0f, -z / 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2.0f, -y / 2.0f, -z / 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2.0f, y / 2.0f, -z / 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2.0f, y / 2.0f, -z / 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},

		//left face
		{glm::vec3(-x / 2, -y / 2, -z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-x / 2, -y / 2, z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, -z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

		//right face
		{glm::vec3(x / 2, -y / 2, -z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, -z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

		//top face
		{glm::vec3(-x / 2, y / 2, -z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, -z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

		//bottom face
		{glm::vec3(-x / 2, -y / 2, -z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, -z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, -y / 2, z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
	};

	GLuint cubeIndices[36] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};

	vertices = new Vertex[24];
	indices = new unsigned int[36];
	memcpy(vertices, cubeVertices, sizeof(cubeVertices));
	memcpy(indices, cubeIndices, sizeof(cubeIndices));
	
	vertexBufferSize = sizeof(cubeVertices);
	indexBufferSize = sizeof(cubeIndices);
}

GLmesh::GLmesh()
{
	vao = vbo = ebo = 0;
	vertexBufferSize = indexBufferSize = 0;
	vertices = nullptr;
	indices = nullptr;
}

GLmesh::GLmesh(MESH_INDEX_ID ID)
{
	vertexBufferSize = 0;
	indexBufferSize = 0;
	vertices = nullptr;
	indices = nullptr;


	switch (ID)
	{
	case CUBE_MESH_ID:
		this->AllocateCubeVertices();
		break;
	}
	

	if ((vertices != nullptr) && (indices != nullptr) && (vertexBufferSize != 0) && (indexBufferSize != 0))
	{

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STATIC_DRAW);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		printf("Error in Mesh.cpp :: Failed to allocate mesh vertices\n");
		vao = 0;
		vbo = 0;
		ebo = 0;
	}
	printf("Loaded mesh id %d\n", ID);
}

GLmesh::~GLmesh()
{
	this->freeBuffers();
}

void GLmesh::setVertexBufferSize(size_t size)
{
	vertexBufferSize = size;
}

void GLmesh::setIndexBufferSize(size_t size)
{
	indexBufferSize = size;
}

GLuint GLmesh::getVAO() const
{
	return vao;
}

GLuint GLmesh::getEBO() const
{
	return ebo;
}



void GLmesh::freeBuffers()
{
	if (vertices != nullptr)
		delete[] vertices;
	if (indices != nullptr)
		delete[] indices;

	vertices = nullptr;
	indices = nullptr;
}

size_t GLmesh::getVertexBufferSize()
{
	return vertexBufferSize;
}

size_t GLmesh::getIndexBufferSize()
{
	return indexBufferSize;
}
