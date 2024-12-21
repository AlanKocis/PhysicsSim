#include <h/Engine.h>

int main()
{
	Engine& engine = Engine::instance();

	while (engine.isRunning())
	{
		engine.updateFrame();	//draw
		engine.updateGUI();
		engine.updateWindow();
	}
	

	engine.End();
	return 0;
} 