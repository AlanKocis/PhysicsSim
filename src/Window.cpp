#include <h/Window.h>

GLFW::Window::Window()
{
	window_ptr = nullptr;
	width = DEFAULT_WINDOW_WIDTH;
	height = DEFAULT_WINDOW_HEIGHT;
	mouse_data.x = width / 2.0;
	mouse_data.y = height / 2.0;
	mouse_data.last_x = mouse_data.x;
	mouse_data.last_y = mouse_data.y;
	first_mouse = true;
	mouse_captured = false;
	cursor_mode = CursorModes::HiddenConfined;
}

GLFW::Window::~Window()
{
	DestroyWindow();
}

void GLFW::Window::Update()
{
	SwapMouseFrameData(GetMouseXPos(), GetMouseYPos());
	glfwSwapBuffers(window_ptr);
	glfwPollEvents();
}

void GLFW::Window::SetTitleText(const char *t)
{
	glfwSetWindowTitle(window_ptr, t);
}

int GLFW::Window::ShouldClose() const
{
	return glfwWindowShouldClose(window_ptr);
}

void GLFW::Window::DestroyWindow()
{
	glfwDestroyWindow(window_ptr);

}

void GLFW::Window::CreateWindow()
{
	if (window_ptr)
		return;

//	center window
	int num_monitors;
	int monitorX, monitorY;

	GLFWmonitor **monitors = glfwGetMonitors(&num_monitors);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	const GLFWvidmode *videoMode = glfwGetVideoMode(monitors[0]);
	glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

	window_ptr = glfwCreateWindow(width, height, "Hobbes v1.2", NULL, NULL);
	assert(window_ptr, "failed to create GLFW::Window\n");

	glfwMakeContextCurrent(window_ptr);
	glfwDefaultWindowHints();
	glfwSetWindowPos(window_ptr,
		monitorX + (videoMode->width - DEFAULT_WINDOW_WIDTH) / 2,
		monitorY + (videoMode->height - DEFAULT_WINDOW_HEIGHT) / 2);
	glfwShowWindow(window_ptr);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Critical error in Window.cpp :: GLAD failed to join window context\n");
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(window_ptr, &(GLFW::Window::FrameBufferSizeCallback));
	glfwSetCursorPosCallback(window_ptr, &(CursorPosCallback));
	glfwSetMouseButtonCallback(window_ptr, &(MouseButtonCallback));
	glfwSetWindowUserPointer(window_ptr, reinterpret_cast<void *>(this));



}

void GLFW::Window::CreateWindow(CursorModes cursor_mode)
{
	CreateWindow();
	SetCursorMode(cursor_mode);
}

bool GLFW::Window::KeyPressed(int key) const
{
	return glfwGetKey(window_ptr, key);
}

void GLFW::Window::SwapMouseFrameData(double x, double y)
{

	mouse_data.last_x = mouse_data.x;
	mouse_data.last_y = mouse_data.y;
	mouse_data.x = x;
	mouse_data.y = y;
}

void GLFW::Window::SwapSizeFrameData(int width, int height)
{
	this->width = width;
	this->height = height;
}

const Mouse &GLFW::Window::GetMouseData()
{
	return mouse_data;
}

void GLFW::Window::FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
//	I hate this, but it is what you are supposed to do lol. This function must be static to bind it as a callback.
	GLFW::Window *user_ptr = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (user_ptr)
		user_ptr->SwapSizeFrameData(width, height);
}

void GLFW::Window::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
	GLFW::Window *user_ptr = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	if (user_ptr)
	{

		user_ptr->SwapMouseFrameData(xpos, ypos);
	}
}

void GLFW::Window::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
}

void GLFW::Window::Init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!glfwInit())
	{
		printf("Error::failed to initialized GLFW\n");
	}
	printf("Initialized GLFW\n");
}

void GLFW::Window::Terminate()
{
	glfwTerminate();
}

double GLFW::Window::GetTime()
{
	return	glfwGetTime();
}

bool GLFW::Window::IsAlive() const
{
	if (!window_ptr)
		return false;
	else
		return true;
}

bool GLFW::Window::IsFirstMouse()
{
	return first_mouse;
}

void GLFW::Window::SetFirstMouse(int val)
{
	first_mouse = val;
}

void GLFW::Window::SetCursorMode(CursorModes mode) const
{
	switch (mode)
	{
	case Visible:
		glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case HiddenConfined:
		glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
		break;
	}
}

double GLFW::Window::GetMouseXOffset() const
{
	return (mouse_data.x - mouse_data.last_x);
}

double GLFW::Window::GetMouseYOffset() const
{
	return (mouse_data.last_y - mouse_data.y);
}

uint32_t GLFW::Window::GetWidth() const
{
	return width;
}

uint32_t GLFW::Window::GetHeight() const
{
	return height;
}
