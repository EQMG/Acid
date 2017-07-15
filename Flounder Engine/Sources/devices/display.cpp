#include "display.h"

namespace flounder {
	display::display() : 
		imodule()
	{
		m_glfwMajor = 3;
		m_glfwMinor = 2;

		m_windowWidth = 720;
		m_windowHeight = 480;
		m_aspectRatio = 1.5f;
		m_fullscreenWidth = 0;
		m_fullscreenHeight = 0;

		m_title = "Flounder C++";
		m_icon = "Resources/flounder.png";
		m_vsync = false;
		m_antialiasing = true;
		m_samples = 0;
		m_fullscreen = false;

		m_window = NULL;
		m_closed = false;
		m_focused = true;
		m_windowPosX = 0;
		m_windowPosY = 0;
	}

	display::~display()
	{
		// Free the window callbacks and destroy the window.
		glfwDestroyWindow(m_window);

		// Terminate GLFW.
		glfwTerminate();

		// delete m_window;

		m_closed = false;
	}

	void display::load(const int glfwMajor, const int glfwMinor, const int width, const int height, const std::string title, std::string icon, const bool vsync, const bool antialiasing, const int samples, const bool fullscreen)
	{
		m_glfwMajor = glfwMajor;
		m_glfwMinor = glfwMinor;

		m_windowWidth = width;
		m_windowHeight = height;
		m_aspectRatio = width / height;

		m_title = title;
		m_icon = icon;
		m_vsync = vsync;
		m_antialiasing = antialiasing;
		m_samples = samples;
		m_fullscreen = fullscreen;
	}

	void display::init()
	{
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
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // The window will stay hidden until after creation.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // The window will be resizable depending on if it's fullscreen.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_glfwMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_glfwMinor);

		// For new GLFW, and macOS.
		if (m_glfwMajor >= 3 && m_glfwMinor >= 2)
		{
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}

		glfwWindowHint(GLFW_STENCIL_BITS, 8); // Fixes 16 bit stencil bits in macOS.
		glfwWindowHint(GLFW_STEREO, GLFW_FALSE); // No stereo view!

		// Use FBO antialiasing instead!
		//if (samples > 0) {
		//	glfwWindowHint(GLFW_SAMPLES, samples); // The number of MSAA samples to use.
		//}

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

		// TODO: Creates a window icon for this GLFW display.
		GLFWimage icons[1];
		{
			int width = 0;
			int height = 0;
			int components = 0;
			stbi_uc *data = stbi_load(m_icon.c_str(), &width, &height, &components, 4);

			if (data == NULL)
			{
				std::cerr << "Unable to load texture: " << data << std::endl;
			}

			icons[0].pixels = data;
			icons[0].width = width;
			icons[0].height = height;

			glfwSetWindowIcon(m_window, 1, icons);
			stbi_image_free(data);
		}

		// Enables VSync if requested.
		glfwSwapInterval(m_vsync ? 1 : 0);

		if (m_vsync) {
			framework::get()->setFpsLimit(60);
		}

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

		// Initialize the GLEW library.
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not init GLEW!" << std::endl;
			framework::get()->requestClose(true);
		}

		// System logs.
		//	logger::get()->log("If you are getting errors, please write a description of how you get the error, and copy this log: https://github.com/Equilibrium-Games/Flounder-Engine/issues");
		//	logger::get()->log("");
		//	logger::get()->log("===== This is not an error message, it is a system info log. =====");
		//	logger::get()->log("Flounder Framework Version: " + framework::get()->getVersion()->getVersion());
		//	logger::get()->log("Flounder Operating System: " + System.getProperty("os.name"));
		//	logger::get()->log("Flounder GLFW Version: " + std::string(glfwGetVersionString()));
		//	logger::get()->log("Flounder OpenGL Version: " + glGetString(GL_VERSION));
		//	logger::get()->log("Flounder OpenGL Vendor: " + glGetString(GL_VENDOR));
		//	logger::get()->log("Flounder Is OpenGL Modern: " + FlounderOpenGL.get().isModern());
		//	logger::get()->log("Flounder Total Memory Available To JVM (bytes): " + Runtime.getRuntime().totalMemory());
		//	logger::get()->log("Flounder Maximum FBO Size: " + glGetInteger(GL_MAX_RENDERBUFFER_SIZE_EXT));
		//	logger::get()->log("Flounder Maximum Texture Size: " + glGetInteger(GL_MAX_TEXTURE_SIZE));
		//	logger::get()->log("Flounder Maximum Anisotropy: " + glGetFloat(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT));
		//	logger::get()->log("===== End of system info log. =====\n");
	}

	void display::update()
	{
		// Swap the colour buffers to the display.
		glfwSwapBuffers(m_window);

		// Polls for window events. The key callback will only be invoked during this call.
		glfwPollEvents();
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

	void display::setWindowSize(const int &width, const int &height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		glfwSetWindowSize(m_window, width, height);
	}

	double &display::getAspectRatio()
	{
		return m_aspectRatio;
	}

	std::string &display::getTitle()
	{
		return m_title;
	}

	bool &display::isVSync()
	{
		return m_vsync;
	}

	void display::setVSync(const bool &vsync)
	{
		m_vsync = vsync;
		glfwSwapInterval(vsync ? 1 : 0);

		if (vsync) {
		//	framework::get()->setFpsLimit(60);
		}
	}

	bool &display::isAntialiasing()
	{
		return m_antialiasing;
	}

	void display::setAntialiasing(const bool &antialiasing)
	{
		m_antialiasing = antialiasing;
	}

	int &display::getSamples()
	{
		return m_samples;
	}

	void display::setSamples(const int &samples)
	{
		m_samples = samples;
		glfwWindowHint(GLFW_SAMPLES, samples);
	}

	bool &display::isFullscreen()
	{
		return m_fullscreen;
	}

	void display::setFullscreen(const bool &fullscreen)
	{
		if (m_fullscreen == fullscreen) {
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
	}

	GLFWwindow *display::getWindow()
	{
		return m_window;
	}

	bool &display::isClosed()
	{
		return m_closed;
	}

	bool &display::isFocused()
	{
		return m_focused;
	}

	int &display::getWindowXPos()
	{
		return m_windowPosX;
	}

	int &display::getWindowYPos()
	{
		return m_windowPosY;
	}
	
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
		display::get()->m_focused = focused == GLFW_TRUE;
	}

	void callbackPosition(GLFWwindow *window, int xpos, int ypos)
	{
		if (!display::get()->m_fullscreen) {
			display::get()->m_windowPosX = xpos;
			display::get()->m_windowPosY = ypos;
		}
	}

	void callbackSize(GLFWwindow *window, int width, int height)
	{
		if (!display::get()->m_fullscreen) {
			display::get()->m_windowWidth = width;
			display::get()->m_windowHeight = height;
			display::get()->m_aspectRatio = width / height;
		}
	}

	void callbackFrame(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}
