#ifndef SHADER_H
#define SHADER_H

#include <glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <h/ResourceDir.h>

enum SHADER_INDEX_ID
{
	CUBE_SHADER_ID,
	PLANE_SHADER_ID,
	NUM_SHADERS
};

class Shader
{
private:
	GLuint ID;
public:
	Shader(const char *vertex_source, const char *fragment_source);
	Shader();
	void loadShaderProgram(const char *vertex_source, const char *fragment_source);
	GLuint getID() const;
	void UseProgram();
//
	void setFloat(const std::string &name, float f);
	void setInt(const std::string &name, int i);
	void setMat4(const std::string& name, const glm::mat4 &M);
	void setMat3(const std::string& name, const glm::mat3 &M);
	void setVec4(const std::string &name, const glm::vec4 &V);
	void setVec3(const std::string &name, const glm::vec3 &V);

};

char* _readfile(FILE* f);
#endif