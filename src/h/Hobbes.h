#ifndef HOBBES_H
#define HOBBES_H

#include <h/Renderer.h>
#include <h/Scene.h>
#include <h/Window.h>
#include <h/Events.h>

namespace Hobbes
{
	struct RenderingEngine
	{
		GLFW::Window window;
		Scene render_scene;
		//Event Log/Queue?
		//GUI
		bool running;
		RenderingEngine();
		~RenderingEngine();
		void Init();
		void Terminate();
		void Run();
	};
};



#endif