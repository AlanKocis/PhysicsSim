#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <h/Defaults.h>
#include <h/Input.h>
#include <string>

namespace GLFW
{
	class Window
	{
	private:
		Mouse mouse_data;
		std::string title;
		GLFWwindow *window_ptr;
		uint32_t width;
		uint32_t height;
		bool first_mouse = true;
	public:
		Window();
		~Window();
		void Update();
		int ShouldClose() const;
		double GetMouseXPos() const { return mouse_data.x; }
		double GetMouseYPos() const { return mouse_data.y; }
		double GetMouseLastXPos() const { return mouse_data.last_x; }
		double GetMouseLastYPos() const { return mouse_data.last_y; }
		void DestroyWindow();
		void CreateWindow();
		void SwapMouseFrameData(double x, double y);
		void SwapSizeFrameData(int width, int height);
		const Mouse &GetMouseData();
		static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
		static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void Init();
		static void Terminate();
		static double GetTime();
		bool IsAlive() const;
		bool IsFirstMouse();
		void SetFirstMouse(int val);
		double GetMouseXOffset() const;
		double GetMouseYOffset() const;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
	};

}


#endif