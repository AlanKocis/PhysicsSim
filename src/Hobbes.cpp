#include <h/Hobbes.h>

Hobbes::RenderingEngine::RenderingEngine()
	: running{ 1 } {};

Hobbes::RenderingEngine::~RenderingEngine()
{
	window.Terminate();
}

void Hobbes::RenderingEngine::Init()
{
	GLFW::Window::Init();
	window.CreateWindow();
	GL::InitRenderer();

	render_scene.LoadDefaultScene();

	running = true;
}

void Hobbes::RenderingEngine::Terminate()
{
	window.Terminate();
}

void Hobbes::RenderingEngine::Run()
{
	double last_time = window.GetTime();
	Init();
	while (running)
	{
		double t = window.GetTime();
		double dt = t - last_time;
		last_time = t;


//		Render pass
		double x = window.GetMouseXOffset();
		double y = window.GetMouseYOffset();
		render_scene.UpdateScene(x, y, dt);

		GL::Viewport(window.GetWidth(), window.GetHeight());
		GL::DrawScene(render_scene);
//		handle events?
//		GUI?
//		This updates mouse data
		window.Update();

//		main window should always be open
		if (window.ShouldClose())
			running = 0;
	}
}