#ifndef RENDERER_H
#define RENDERER_H
#include <h/Engine.h>
#include <h/World.h>
#include <glm/gtc/type_ptr.hpp>

namespace Renderer
{
	void DrawWorld(World& world);
	void DrawCube(Cube& cube);
}



#endif