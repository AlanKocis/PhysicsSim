#ifndef ENGINE_H
#define ENGINE_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stdlib.h>
#include <stdio.h>

#include <h/Camera.h>
#include <h/Mesh.h>
#include <h/ResourceDir.h>
#include <h/Shader.h>
#include <h/World.h>
#include <h/ResourceManager.h>

#define INIT_WINDOW_WIDTH 1600
#define INIT_WINDOW_HEIGHT 900

void _glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height);
void _glfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos);
void _glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

enum ENGINE_CONFIG_ID
{
	CONFIG_1,
	NUM_CONFIGS
};

enum MOUSE_POS_INDEX_ID
{
	X,
	Y
};


class Engine
{
private:
	ResourceManager resourceManager;
	int windowWidth;
	int windowHeight;
	double lastMousePos[2];
	GLFWwindow *window;
	World *worldRenderTarget;
	bool running = true;
	Engine(ENGINE_CONFIG_ID ID);
public:
	static Engine &instance();
	World *loadWorld(const WORLD_TYPE &ID);
	const World *getWorldRenderTarget();
	void updateFrame();
	void updateWindow();
	void updateGUI();
	void processKeyboardInput(GLFWwindow *window);
	bool isRunning();
	double getLastMousePos(const MOUSE_POS_INDEX_ID& index);
	void setLastMousePosition(const double &x, const double &y);
	void End();
};
#endif