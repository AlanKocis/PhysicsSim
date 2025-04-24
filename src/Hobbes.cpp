#include <h/Hobbes.h>

Hobbes::RenderingEngine::RenderingEngine()
	: running{ 1 } {}

Hobbes::RenderingEngine::~RenderingEngine()
{
	window.Terminate();
}

void Hobbes::RenderingEngine::Init()
{
	GLFW::Window::Init();
	window.CreateWindow(GLFW::CursorModes::HiddenConfined);
	GL::InitRenderer();
	render_scene.LoadIDTestScene();
	EventQueue &_InitEventQSingleton = EventQueue::Instance();
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
//	main "game loop"

	 // FPS tracking variables
	int frame_count = 0;
	double fps_timer = 0.0;
	double avg_fps = 0.0;

	char str[50];


	while (running)
	{
		double t = window.GetTime();
		double dt = t - t_naught;
		t_naught = t;

		// Update FPS tracking
		frame_count++;
		fps_timer += dt;

		// Calculate average FPS every second
		if (fps_timer >= 1.0)
		{
			avg_fps = frame_count / fps_timer;  // Frames / seconds
			frame_count = 0;
			fps_timer = 0.0;
		}

		snprintf(str, 50, "%d entities  -  %.3f fps", EntityManager::GetNumActiveEntities(), avg_fps);
		//snprintf(str, 50, "%d entities  -  %.3f fps", (int)render_scene.cube_entities.size(), avg_fps);

		window.SetTitleText(str);


//		Render pass
		render_scene.UpdateScene(window, dt);
		GL::DrawScene_ID_instanced(render_scene);

		uint32_t w, h;
		w = window.GetWidth();
		h = window.GetHeight();
		GL::Viewport(w, h);

//		handle events?
//		GUI?
//		This updates mouse data
		window.Update();

		


//		main window should always be open
		if (window.ShouldClose())
			running = 0;
	}
}