#include <h/Engine.h>

int main()
{
	Engine& GL_Engine = Engine::instance();
	while (GL_Engine.isRunning())
	{
		GL_Engine.updateFrame();
		GL_Engine.updateGUI();
		GL_Engine.updateWindow();
	}


	GL_Engine.End();
	return 0;
}