#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 world;

out vec3 _pos;

void main()
{
	gl_Position = proj * view * world * vec4(aPos, 1.0);
	_pos = aPos;
	//gl_Position = vec4(aPos, 1.0);
}