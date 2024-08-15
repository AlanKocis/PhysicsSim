#include <h/Engine.h>

int main()
{
	Engine& core = Engine::instance();

	while (core.isRunning())
	{
		core.updateFrame();	//draw
		core.updateGUI();
		core.updateWindow();
	}
	 

	core.End();
	return 0;
} 