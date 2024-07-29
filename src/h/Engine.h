#ifndef ENGINE_H
#define ENGINE_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stdlib.h>
#include <stdio.h>

#include <h/Renderer.h>
#include <h/Camera.h>
#include <h/Mesh.h>
#include <h/ResourceDir.h>
#include <h/Shader.h>
#include <h/World.h>
#include <h/ResourceManager.h>

#define INIT_WINDOW_WIDTH 1600
#define INIT_WINDOW_HEIGHT 900
#define INIT_MOUSE_MODE GLFW_CURSOR_DISABLED


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

struct Mouse
{
	int mouseModes[2] = {GLFW_CURSOR_DISABLED, GLFW_CURSOR_NORMAL};
	bool currentModeFlag = 0;
};

class Engine
{
private:
	ResourceManager resourceManager;
	Mouse mouseData{};
	float lastFrameTime, deltaTime, FPS;
	int windowWidth;
	int windowHeight;
	int captureMouseIndex;
	double lastMousePos[2];
	GLFWwindow *window;
	World *worldRenderTarget;
	bool running, firstRun, keyboardInputFlag;
	Engine(ENGINE_CONFIG_ID ID);
public:
	static Engine &instance();
	ResourceManager &getResourceManager();
	World *loadWorld(const WORLD_TYPE &ID);
	World *getWorldRenderTarget();
	void updateFrame();
	void updateWindow();
	void updateGUI();
	void processKeyboardInput();
	bool isRunning();
	double getLastMousePos(const MOUSE_POS_INDEX_ID& index);
	void setLastMousePosition(const double &x, const double &y);
	void setWindowWidth(const int &w);
	void setWindowHeight(const int &h);
	float& getDeltaTime();
	float& getFPS();
	void End();
	bool shouldMoveMouse();
};
#endif