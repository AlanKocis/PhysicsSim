#include <h/Mesh.h>

void copyCubeMeshData(Mesh *mesh, Vertex *vertices, GLuint *indices)
{
	float x = 1.0f, y = 1.0f, z = 1.0f;

	std::vector<Vertex> cubeVertices{
		//front face
		//pos			normal				tex
		{glm::vec3(-x / 2, -y / 2, z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},

		//back face
		{glm::vec3(-x / 2, -y / 2, -z / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, -z / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, -z / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, -z / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},

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

	memcpy(vertices, cubeVertices.data(), sizeof(Vertex) * 24);
	memcpy(indices, cubeIndices, sizeof(cubeIndices));
	mesh->setVertexBufferSize(sizeof(Vertex) * 24);
	mesh->setIndexBufferSize(sizeof(cubeIndices));
}

Mesh::Mesh()
{
	vao = vbo = ebo = 0;
	vertexBufferSize = indexBufferSize = 0;
	vertices = nullptr;
	indices = nullptr;
}

Mesh::Mesh(MESH_INDEX_ID ID)
{
	vertexBufferSize = 0;
	indexBufferSize = 0;
	vertices = nullptr;
	indices = nullptr;
	switch (ID)
	{
	case CUBE_MESH_ID:
		vertices = new Vertex[24];
		indices = new GLuint[36];
		copyCubeMeshData(this, vertices, indices);
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
	}
	else
	{
		vao = 0;
		vbo = 0;
		ebo = 0;
	}
}

void Mesh::setVertexBufferSize(size_t size)
{
	vertexBufferSize = size;
}

void Mesh::setIndexBufferSize(size_t size)
{
	indexBufferSize = size;
}

GLuint Mesh::getVAO()
{
	return vao;
}



void Mesh::freeBuffers()
{
	if (vertices != nullptr)
		delete[] vertices;
	if (indices != nullptr)
		delete[] indices;

	vertices = nullptr;
	indices = nullptr;
}

size_t Mesh::getVertexBufferSize()
{
	return vertexBufferSize;
}

size_t Mesh::getIndexBufferSize()
{
	return indexBufferSize;
}
