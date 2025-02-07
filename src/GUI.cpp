#include <h/GUI.h>

void GUI::Init(GLFWwindow *window, const std::string &ogl_version)
{
	LinkGLFWWindow(window);
	LinkOpenGLVersion(ogl_version);
	ImGui::StyleColorsDark();

}

void GUI::LinkGLFWWindow(GLFWwindow *window)
{
	ImGui_ImplGlfw_InitForOpenGL(window, true);
}

void GUI::LinkOpenGLVersion(const std::string &version_str)
{
	ImGui_ImplOpenGL3_Init("#version 330");

}
