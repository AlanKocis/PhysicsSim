#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <h/Defaults.h>
#include <h/Input.h>
#include <cassert>
#include <string>
#include <unordered_map>

namespace GLFW
{
	enum CursorModes
	{
		Visible,
		HiddenConfined,
	};

	class Window
	{
	private:
		Mouse mouse_data;
		std::string title;
		GLFWwindow *window_ptr;
		uint32_t width;
		uint32_t height;
		CursorModes cursor_mode;
		bool first_mouse;
		bool mouse_captured;
	public:
		Window();
		~Window();
//
		void Update();
		void DestroyWindow();
		void CreateWindow();
		void CreateWindow(CursorModes cursor_mode);
//
		bool KeyPressed(int key) const;
		double GetMouseXPos() const { return mouse_data.x; }
		double GetMouseYPos() const { return mouse_data.y; }
		double GetMouseLastXPos() const { return mouse_data.last_x; }
		double GetMouseLastYPos() const { return mouse_data.last_y; }
		double GetMouseXOffset() const;
		double GetMouseYOffset() const;
		const Mouse &GetMouseData();
		int ShouldClose() const;
		bool IsAlive() const;
		bool IsFirstMouse();
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
//
		void SetCursorMode(CursorModes cursor_mode) const;
		void SetFirstMouse(int val);

//
		void SwapMouseFrameData(double x, double y);
		void SwapSizeFrameData(int width, int height);
//		GLOBAL GLFW FUNCTIONS
		static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
		static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void Init(); 
		static void Terminate();
		static double GetTime();
//
	};

}


#endif