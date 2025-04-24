#ifndef RENDERER_H
#define RENDERER_H

#include <glad.h>
#include <h/Scene.h>

namespace GL
{
	void InitRenderer();
	void StartRenderPass();
	void DrawScene(const Scene& scene);
	void DrawScene_ID(Scene& scene);
	void DrawScene_ID_instanced(Scene &scene);
	void DrawScene_ptr_instanced(Scene &scene);
	void Viewport(int width, int height);
};

#endif