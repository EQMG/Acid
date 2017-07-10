#pragma once

#include <String>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../framework/framework.h"
#include "../logger/logger.h"

#include "../textures/stb_image.h"

namespace flounder {
	/// <summary>
	/// A module used for the creation, updating and destruction of the display.
	/// </summary>
	class display : public imodule
	{
	private:
		int m_glfwMajor;
		int m_glfwMinor;

		int m_windowWidth;
		int m_windowHeight;
		int m_fullscreenWidth;
		int m_fullscreenHeight;

		std::string m_title;
		std::string m_icon;
		bool m_vsync;
		bool m_antialiasing;
		int m_samples;
		bool m_fullscreen;
		
		GLFWwindow *m_window;
		bool m_closed;
		bool m_focused;
		int m_windowPosX;
		int m_windowPosY;

		friend static void callbackError(int error, const char* description);
		friend static void callbackClose(GLFWwindow* window);
		friend static void callbackFocus(GLFWwindow* window, int focused);
		friend static void callbackPosition(GLFWwindow* window, int xpos, int ypos);
		friend static void callbackSize(GLFWwindow* window, int width, int height);
		friend static void callbackFrame(GLFWwindow* window, int width, int height);
	public:
		static display* get() 
		{
			return (display*) framework::get()->getInstance("display");
		}

		/// <summary>
		/// Creates a new GLFW display manager.
		/// </summary>
		display();

		/// <summary>
		/// Deconstructor for the display.
		/// </summary>
		~display();

		void load(const int glfwMajor, const int glfwMinor, const int width, const int height, const std::string title, std::string icon, const bool vsync, const bool antialiasing, const int samples, const bool fullscreen);

		void init();

		void update();

		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into the screenshots folder a png image.
		/// </summary>
		void screenshot();

		/// <summary>
		/// Gets the width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		int getWidth();

		int getWindowWidth();

		/// <summary>
		/// Gets the height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		int getHeight();

		int getWindowHeight();

		void setWindowSize(int width, int height);

		/// <summary>
		/// Gets the aspect ratio between the displays width and height.
		/// </summary>
		/// <returns> The aspect ratio. </returns>
		double getAspectRatio();

		/// <summary>
		/// Gets the window's title.
		/// </summary>
		/// <returns> The window's title. </returns>
		std::string getTitle();

		/// <summary>
		/// Gets if the display is using vSync.
		/// </summary>
		/// <returns> If VSync is enabled. </returns>
		bool isVSync();

		/// <summary>
		/// Sets the display to use VSync or not.
		/// </summary>
		/// <param name="vsync"> Weather or not to use vSync. </param>
		void setVSync(bool vsync);

		/// <summary>
		/// Gets if the display requests antialiased images.
		/// </summary>
		/// <returns> If using antialiased images. </returns>
		bool isAntialiasing();

		/// <summary>
		/// Requests the display to antialias.
		/// </summary>
		/// <param name="antialiasing"> If the display should antialias. </param>
		void setAntialiasing(bool antialiasing);

		/// <summary>
		/// Gets how many MSAA samples should be done before swapping buffers.
		/// </summary>
		/// <returns> Amount of MSAA samples. </returns>
		int getSamples();

		/// <summary>
		/// Gets how many MSAA samples should be done before swapping buffers. Zero disables multisampling. GLFW_DONT_CARE means no preference.
		/// </summary>
		/// <param name="samples"> The amount of MSAA samples. </param>
		void setSamples(int samples);

		/// <summary>
		/// Gets weather the display is fullscreen or not.
		/// </summary>
		/// <returns> Fullscreen or windowed. </returns>
		bool isFullscreen();

		/// <summary>
		/// Sets the display to be fullscreen or windowed.
		/// </summary>
		/// <param name="fullscreen"> Weather or not to be fullscreen. </param>
		void setFullscreen(bool fullscreen);

		/// <summary>
		/// Gets the current GLFW window.
		/// </summary>
		/// <returns> The current GLFW window. </returns>
		GLFWwindow *getWindow();

		/// <summary>
		/// Gets if the GLFW display is closed.
		/// </summary>
		/// <returns> If the GLFW display is closed. </returns>
		bool isClosed();

		/// <summary>
		/// Gets if the GLFW display is selected.
		/// </summary>
		/// <returns> If the GLFW display is selected. </returns>
		bool isFocused();

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows x position. </returns>
		int getWindowXPos();

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows Y position. </returns>
		int getWindowYPos();
	};
}
