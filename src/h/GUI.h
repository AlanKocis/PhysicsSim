#ifndef GUI_H
#define GUI_H

#include <h/Window.h>
#include <h/Entity.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class GUI
{
private:
	void LinkGLFWWindow(GLFWwindow *window);
	void LinkOpenGLVersion(const std::string &version_str);
public:
	void Init(GLFWwindow *window, const std::string &ogl_version);
	void DisplayCubeEntityData(const CubeEntity &entity);
};





#endif