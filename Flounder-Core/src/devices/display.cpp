#include "display.h"

namespace flounder
{
	void callbackError(int error, const char *description)
	{
		fprintf(stderr, "GLFW Error: %s\n", description);
	}

	void callbackClose(GLFWwindow *window)
	{
		display::get()->m_closed = false;
		framework::get()->requestClose(false);
	}

	void callbackFocus(GLFWwindow *window, int focused)
	{
		display::get()->m_focused = focused;
	}

	void callbackPosition(GLFWwindow *window, int xpos, int ypos)
	{
		if (!display::get()->m_fullscreen)
		{
			display::get()->m_windowPosX = xpos;
			display::get()->m_windowPosY = ypos;
		}
	}

	void callbackSize(GLFWwindow *window, int width, int height)
	{
		if (display::get()->m_fullscreen)
		{
			display::get()->m_fullscreenWidth = width;
			display::get()->m_fullscreenHeight = height;
		}
		else
		{
			display::get()->m_windowWidth = width;
			display::get()->m_windowHeight = height;
		}
	}

	void callbackFrame(GLFWwindow *window, int width, int height)
	{
		display::get()->m_aspectRatio = width / height;
		glViewport(0, 0, width, height);
	}

	display::display() :
		imodule()
	{
		m_glfwMajor = 3;
		m_glfwMinor = 2;

		m_windowWidth = 1080;
		m_windowHeight = 720;
		m_fullscreenWidth = 0;
		m_fullscreenHeight = 0;

		m_title = "Flounder C++";
		m_icon = "";
		m_fpsLimit = -1.0f;
		m_vsync = false;
		m_antialiasing = true;
		m_samples = 0;
		m_fullscreen = false;

		m_window = NULL;
		m_closed = false;
		m_focused = true;
		m_windowPosX = 0;
		m_windowPosY = 0;

#ifdef FLOUNDER_PLATFORM_WEB
		m_fpsLimit = 60.0f;
		m_vsync = true;
#endif

		// Set the error error callback
		glfwSetErrorCallback(callbackError);

		// Initialize the GLFW library.
		if (!glfwInit())
		{
			std::cout << "Could not init GLFW!" << std::endl;
			framework::get()->requestClose(true);
		}

		// Configures the window.
		glfwDefaultWindowHints();
#ifndef FLOUNDER_PLATFORM_WEB
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // The window will stay hidden until after creation.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // The window will be resizable depending on if it's fullscreen.
#endif
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_glfwMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_glfwMinor);

		// For new GLFW, and macOS.
		if (m_glfwMajor >= 3 && m_glfwMinor >= 2)
		{
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}

		glfwWindowHint(GLFW_STENCIL_BITS, 8); // Fixes 16 bit stencil bits in macOS.
#ifndef FLOUNDER_PLATFORM_WEB
		glfwWindowHint(GLFW_STEREO, GLFW_FALSE); // No stereo view!
#endif

		// Get the resolution of the primary monitor.
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		if (m_fullscreen)
		{
			m_fullscreenWidth = videoMode->width;
			m_fullscreenHeight = videoMode->height;
			m_aspectRatio = videoMode->width / videoMode->height;
		}

		// Create a windowed mode window and its OpenGL context.
		m_window = glfwCreateWindow(m_fullscreen ? m_fullscreenWidth : m_windowWidth, m_fullscreen ? m_fullscreenHeight : m_windowHeight, m_title.c_str(), m_fullscreen ? monitor : NULL, NULL);
		m_closed = false;
		m_focused = true;

		// Gets any window errors.
		if (m_window == NULL)
		{
			std::cout << "Could not create the window! Update your graphics drivers and ensure your computer supports OpenGL!" << std::endl;
			framework::get()->requestClose(true);
			glfwTerminate();
		}

		// Centre the window position.
		m_windowPosX = (videoMode->width - m_windowWidth) / 2;
		m_windowPosY = (videoMode->height - m_windowHeight) / 2;
		glfwSetWindowPos(m_window, m_windowPosX, m_windowPosY);

		// Creates the OpenGL context.
		glfwMakeContextCurrent(m_window);

		// Enables VSync if requested.
		glfwSwapInterval(m_vsync ? 1 : 0);

		// Shows the OpenGl window.
		glfwShowWindow(m_window);

		// Gets any OpenGL errors.
		GLenum glError = glGetError();

		if (glError != GL_NO_ERROR)
		{
			std::cout << "Failed to load OpenGL!" << std::endl << glError << std::endl;
			framework::get()->requestClose(true);
		}

		// Sets the displays callbacks.
		glfwSetWindowCloseCallback(m_window, callbackClose);
		glfwSetWindowFocusCallback(m_window, callbackFocus);
		glfwSetWindowPosCallback(m_window, callbackPosition);
		glfwSetWindowSizeCallback(m_window, callbackSize);
		glfwSetFramebufferSizeCallback(m_window, callbackFrame);

#ifndef FLOUNDER_PLATFORM_WEB
		// Initialize the GLEW library.
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not init GLEW!" << std::endl;
			framework::get()->requestClose(true);
		}
#endif
	}

	display::~display()
	{
		// Free the window callbacks and destroy the window.
		glfwDestroyWindow(m_window);

		// Terminate GLFW.
		glfwTerminate();

		m_closed = false;
	}

	void display::update()
	{
		// Swap the colour buffers to the display.
		glfwSwapBuffers(m_window);

		// Polls for window events. The key callback will only be invoked during this call.
		glfwPollEvents();

		// Updates the aspect ratio.
		m_aspectRatio = static_cast<float>(getWidth()) / static_cast<float>(getHeight());
	}

	void display::screenshot()
	{
		// TODO
	}

	int &display::getWidth()
	{
		return m_fullscreen ? m_fullscreenWidth : m_windowWidth;
	}

	int &display::getWindowWidth()
	{
		return m_windowWidth;
	}

	int &display::getHeight()
	{
		return m_fullscreen ? m_fullscreenHeight : m_windowHeight;
	}

	int &display::getWindowHeight()
	{
		return m_windowHeight;
	}

	double &display::getAspectRatio()
	{
		return m_aspectRatio;
	}

	void display::setWindowSize(const int &width, const int &height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		glfwSetWindowSize(m_window, width, height);
	}

	std::string &display::getTitle()
	{
		return m_title;
	}

	void display::setTitle(const std::string & title)
	{
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.c_str());
	}

	std::string &display::getIcon()
	{
		return m_icon;
	}

	void display::setIcon(const std::string &icon)
	{
		// Creates a window icon for this GLFW display.
#ifndef FLOUNDER_PLATFORM_WEB
		m_icon = icon;

		if (!m_icon.empty())
		{
			int width = 0;
			int height = 0;
			int components = 0;
			stbi_uc *data = stbi_load(m_icon.c_str(), &width, &height, &components, 4);

			if (data == NULL)
			{
				std::cout << "Unable to load texture: " << m_icon << std::endl;
			}
			else
			{
				GLFWimage icons[1];
				icons[0].pixels = data;
				icons[0].width = width;
				icons[0].height = height;

				glfwSetWindowIcon(m_window, 1, icons);
			}

			stbi_image_free(data);
		}
#endif
	}

	int display::getFpsLimit()
	{
		return m_fpsLimit;
	}

	void display::setFpsLimit(const int &fpsLimit)
	{
#ifndef FLOUNDER_PLATFORM_WEB
		m_fpsLimit = fpsLimit;
#else
		m_fpsLimit = 60;
#endif
	}

	bool display::isVSync()
	{
		return m_vsync;
	}

	void display::setVSync(const bool &vsync)
	{
#ifndef FLOUNDER_PLATFORM_WEB
		m_vsync = vsync;
#else
		m_vsync = true;
#endif

		glfwSwapInterval(vsync ? 1 : 0);
	}

	bool display::isAntialiasing()
	{
		return m_antialiasing;
	}

	void display::setAntialiasing(const bool &antialiasing)
	{
		m_antialiasing = antialiasing;
	}

	int display::getSamples()
	{
		return m_samples;
	}

	void display::setSamples(const int &samples)
	{
		m_samples = samples;
		glfwWindowHint(GLFW_SAMPLES, samples);
	}

	bool display::isFullscreen()
	{
		return m_fullscreen;
	}

	void display::setFullscreen(const bool &fullscreen)
	{
#ifndef FLOUNDER_PLATFORM_WEB
		if (m_fullscreen == fullscreen)
		{
			return;
		}

		m_fullscreen = fullscreen;
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		std::cout << (fullscreen ? "Display is going fullscreen." : "Display is going windowed.") << std::endl;

		if (fullscreen)
		{
			m_fullscreenWidth = videoMode->width;
			m_fullscreenHeight = videoMode->height;
			glfwSetWindowMonitor(m_window, monitor, 0, 0, m_fullscreenWidth, m_fullscreenHeight, GLFW_DONT_CARE);
		}
		else
		{
			m_windowPosX = (videoMode->width - m_windowWidth) / 2;
			m_windowPosY = (videoMode->height - m_windowHeight) / 2;
			glfwSetWindowMonitor(m_window, NULL, m_windowPosX, m_windowPosY, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
		}
#endif
	}

	GLFWwindow *display::getWindow()
	{
		return m_window;
	}

	bool display::isClosed()
	{
		return m_closed;
	}

	bool display::isFocused()
	{
		return m_focused;
	}

	int display::getWindowXPos()
	{
		return m_windowPosX;
	}

	int display::getWindowYPos()
	{
		return m_windowPosY;
	}
}
