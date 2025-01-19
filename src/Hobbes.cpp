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
	window.CreateWindow(GLFW::CursorModes::HiddenConfined);
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
	double t_naught = window.GetTime();
	Init();
	while (running)
	{
		double t = window.GetTime();
		double dt = t - t_naught;
		t_naught = t;

//		Render pass
		render_scene.UpdateScene(window, dt);

		uint32_t w, h;
		w = window.GetWidth();
		h = window.GetHeight();
		GL::Viewport(w, h);
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