#ifndef SHADER_H
#define SHADER_H

#include <glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <h/ResourceDir.h>

enum SHADER_INDEX_ID
{
	CUBE_SHADER_ID,
	CUBE_OUTLINE_ID,
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
	GLuint &getID();

};

char* _readfile(FILE* f);
#endif