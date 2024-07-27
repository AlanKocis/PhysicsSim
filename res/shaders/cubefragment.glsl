#version 330 core

in vec3 _pos;
out vec4 FragColor;

void main()
{
	FragColor = vec4(_pos, 1.0);
}