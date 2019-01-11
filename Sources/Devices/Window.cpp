#include "Window.hpp"

#include <cassert>
#include <GLFW/glfw3.h>
#include "Renderer/Renderer.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	void CallbackError(int32_t error, const char *description)
	{
		Window::CheckGlfw(error);
		Log::Error("GLFW error: %s, %i\n", description, error);
	}

	void CallbackMonitor(GLFWmonitor *monitor, int32_t event)
	{
		Log::Error("Monitor action: %s, %i\n", glfwGetMonitorName(monitor), event);
	}

	void CallbackClose(GLFWwindow *window)
	{
		Window::Get()->m_closed = false;
		Engine::Get()->RequestClose(false);
	}

	void CallbackFocus(GLFWwindow *window, int32_t focused)
	{
		Window::Get()->m_focused = static_cast<bool>(focused);
	}

	void CallbackPosition(GLFWwindow *window, int32_t xpos, int32_t ypos)
	{
		if (!Window::Get()->m_fullscreen)
		{
			Window::Get()->m_positionX = static_cast<uint32_t>(xpos);
			Window::Get()->m_positionY = static_cast<uint32_t>(ypos);
		}
	}

	void CallbackSize(GLFWwindow *window, int32_t width, int32_t height)
	{
		if (width <= 0 || height <= 0)
		{
			return;
		}

		if (Window::Get()->m_fullscreen)
		{
			Window::Get()->m_fullscreenWidth = static_cast<uint32_t>(width);
			Window::Get()->m_fullscreenHeight = static_cast<uint32_t>(height);
		}
		else
		{
			Window::Get()->m_windowWidth = static_cast<uint32_t>(width);
			Window::Get()->m_windowHeight = static_cast<uint32_t>(height);
		}

		Window::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		Renderer::Get()->UpdateSurfaceCapabilities();
	}

	void CallbackFrame(GLFWwindow *window, int32_t width, int32_t height)
	{
		Window::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths)
	{
		std::vector<std::string> files(static_cast<uint32_t>(count));

		for (uint32_t i = 0; i < count; i++)
		{
			files[i] = paths[i];
		}

		Window::Get()->m_onDrop(files);
	}

	void CallbackIconify(GLFWwindow *window, int32_t iconified)
	{
		Window::Get()->m_iconified = iconified == GLFW_TRUE;
	}

	Window::Window() :
		m_windowWidth(1080),
		m_windowHeight(720),
		m_fullscreenWidth(0),
		m_fullscreenHeight(0),
		m_aspectRatio(1.5f),
		m_positionX(0),
		m_positionY(0),
		m_title("Acid Loading..."),
		m_iconPath(""),
		m_borderless(false),
		m_resizable(true),
		m_floating(false),
		m_fullscreen(false),
		m_closed(false),
		m_focused(true),
		m_iconified(false),
		m_onDrop(Delegate<void(std::vector<std::string>)>()),
		m_window(nullptr)
	{
		// Set the error error callback
		glfwSetErrorCallback(CallbackError);

		// Initialize the GLFW library.
		if (glfwInit() == GLFW_FALSE)
		{
			Log::Error("GLFW error: Failed to initialize!\n");
			assert(false && "GLFW runtime error!");
		}

		// Checks Vulkan support on GLFW.
		if (glfwVulkanSupported() == GLFW_FALSE)
		{
			Log::Error("GLFW error: Failed to find Vulkan support!\n");
			assert(false && "GLFW runtime error!");
		}

		// Set the monitor callback
		glfwSetMonitorCallback(CallbackMonitor);

		// Configures the window.
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // The window will stay hidden until after creation.

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable context creation.

		// For new GLFW, and macOS.
		glfwWindowHint(GLFW_STENCIL_BITS, 8); // Fixes 16 bit stencil bits in macOS.
		glfwWindowHint(GLFW_STEREO, GLFW_FALSE); // No stereo view!

		// Get the resolution of the primary monitor.
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		if (m_fullscreen)
		{
			m_fullscreenWidth = static_cast<uint32_t>(videoMode->width);
			m_fullscreenHeight = static_cast<uint32_t>(videoMode->height);
			m_aspectRatio = static_cast<float>(videoMode->width) / static_cast<float>(videoMode->height);
		}

		//	glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
		//	glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
		//	glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
		//	glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

		// Create a windowed mode window and its context.
		m_window = glfwCreateWindow(m_fullscreen ? m_fullscreenWidth : m_windowWidth, m_fullscreen ? m_fullscreenHeight : m_windowHeight, m_title.c_str(), m_fullscreen ? monitor : nullptr, nullptr);

		// Gets any window errors.
		if (m_window == nullptr)
		{
			glfwTerminate();
			assert(false && "Filed to create the GLFW window!");
		}

		// Sets the user pointer.
		glfwSetWindowUserPointer(m_window, this);

		// Window attributes that can change later.
		glfwSetWindowAttrib(m_window, GLFW_DECORATED, m_borderless ? GLFW_FALSE : GLFW_TRUE);
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
		glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating ? GLFW_TRUE : GLFW_FALSE);

		// Centre the window position.
		m_positionX = (videoMode->width - m_windowWidth) / 2;
		m_positionY = (videoMode->height - m_windowHeight) / 2;
		glfwSetWindowPos(m_window, m_positionX, m_positionY);

		// Shows the glfw window.
		glfwShowWindow(m_window);

		// Sets the displays callbacks.
		glfwSetWindowCloseCallback(m_window, CallbackClose);
		glfwSetWindowFocusCallback(m_window, CallbackFocus);
		glfwSetWindowPosCallback(m_window, CallbackPosition);
		glfwSetWindowSizeCallback(m_window, CallbackSize);
		glfwSetWindowIconifyCallback(m_window, CallbackIconify);
		glfwSetFramebufferSizeCallback(m_window, CallbackFrame);
		glfwSetDropCallback(m_window, CallbackDrop);
	}

	Window::~Window()
	{
		// Free the window callbacks and destroy the window.
		glfwDestroyWindow(m_window);

		// Terminate GLFW.
		glfwTerminate();

		m_closed = true;
	}

	void Window::Update()
	{
		// Polls for window events.
		glfwPollEvents();
	}

	void Window::SetDimensions(const uint32_t &width, const uint32_t &height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		glfwSetWindowSize(m_window, width, height);
	}

	void Window::SetDimensions(const Vector2 &size)
	{
		SetDimensions(size.m_x == -1.0f ? GetWidth() : static_cast<uint32_t>(size.m_x),
			size.m_y == -1.0f ? GetHeight() : static_cast<uint32_t>(size.m_y));
	}

	void Window::SetPosition(const uint32_t &x, const uint32_t &y)
	{
		glfwSetWindowPos(m_window, x, y);
	}

	void Window::SetPosition(const Vector2 &position)
	{
		SetPosition(position.m_x == -1.0f ? GetPositionX() : static_cast<uint32_t>(position.m_x),
			position.m_y == -1.0f ? GetPositionY() : static_cast<uint32_t>(position.m_y));
	}

	void Window::SetTitle(const std::string &title)
	{
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.c_str());
	}

	void Window::SetIcon(const std::string &filename)
	{
		m_iconPath = filename;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t components = 0;
		uint8_t *data = Texture::LoadPixels(m_iconPath, &width, &height, &components);

		if (data == nullptr)
		{
			return;
		}

		GLFWimage icons[1];
		icons[0].pixels = data;
		icons[0].width = width;
		icons[0].height = height;

		glfwSetWindowIcon(m_window, 1, icons);
		Texture::DeletePixels(data);
	}

	void Window::SetBorderless(const bool &borderless)
	{
		m_borderless = borderless;
		glfwSetWindowAttrib(m_window, GLFW_DECORATED, m_borderless ? GLFW_FALSE : GLFW_TRUE);
	}

	void Window::SetResizable(const bool &resizable)
	{
		m_resizable = resizable;
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
	}

	void Window::SetFloating(const bool &floating)
	{
		m_floating = floating;
		glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating ? GLFW_TRUE : GLFW_FALSE);
	}

	void Window::SetFullscreen(const bool &fullscreen)
	{
		if (m_fullscreen == fullscreen)
		{
			return;
		}

		m_fullscreen = fullscreen;

	//  int32_t monitorCount;
	//  GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

		GLFWmonitor *monitor = glfwGetPrimaryMonitor(); // monitors[1];
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		if (fullscreen)
		{
#if defined(ACID_VERBOSE)
			printf("Window is going fullscreen\n");
#endif
			m_fullscreenWidth = static_cast<uint32_t>(videoMode->width);
			m_fullscreenHeight = static_cast<uint32_t>(videoMode->height);
			glfwSetWindowMonitor(m_window, monitor, 0, 0, m_fullscreenWidth, m_fullscreenHeight, GLFW_DONT_CARE);
		}
		else
		{
#if defined(ACID_VERBOSE)
			printf("Window is going windowed\n");
#endif
			m_positionX = (videoMode->width - m_windowWidth) / 2;
			m_positionY = (videoMode->height - m_windowHeight) / 2;
			glfwSetWindowMonitor(m_window, nullptr, m_positionX, m_positionY, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
		}
	}

	void Window::SetIconified(const bool &iconify)
	{
		if (!m_iconified && iconify)
		{
			glfwIconifyWindow(m_window);
		}
		else if (m_iconified && !iconify)
		{
			glfwRestoreWindow(m_window);
		}
	}

	std::string Window::GetClipboard() const
	{
		return glfwGetClipboardString(m_window);
	}

	void Window::SetClipboard(const std::string &string) const
	{
		glfwSetClipboardString(m_window, string.c_str());
	}

	std::string Window::StringifyResultGlfw(const int32_t &result)
	{
		switch (result)
		{
			case GLFW_TRUE:
				return "Success";
			case GLFW_NOT_INITIALIZED:
				return "GLFW has not been initialized";
			case GLFW_NO_CURRENT_CONTEXT:
				return "No context is current for this thread";
			case GLFW_INVALID_ENUM:
				return "One of the arguments to the function was an invalid enum value";
			case GLFW_INVALID_VALUE:
				return "One of the arguments to the function was an invalid value";
			case GLFW_OUT_OF_MEMORY:
				return "A memory allocation failed";
			case GLFW_API_UNAVAILABLE:
				return "GLFW could not find support for the requested API on the system";
			case GLFW_VERSION_UNAVAILABLE:
				return "The requested OpenGL or OpenGL ES version is not available";
			case GLFW_PLATFORM_ERROR:
				return "A platform-specific error occurred that does not match any of the more specific categories";
			case GLFW_FORMAT_UNAVAILABLE:
				return "The requested format is not supported or available";
			case GLFW_NO_WINDOW_CONTEXT:
				return "The specified window does not have an OpenGL or OpenGL ES context";
			default:
				return "ERROR: UNKNOWN GLFW ERROR";
		}
	}

	void Window::CheckGlfw(const int32_t &result)
	{
		if (result == GLFW_TRUE)
		{
			return;
		}

		std::string failure = StringifyResultGlfw(result);

		Log::Error("GLFW error: %s, %i\n", failure.c_str(), result);
		Log::Popup("GLFW Error", failure);
		assert(false && "GLFW error!");
	}

	std::pair<const char **, uint32_t> Window::GetInstanceExtensions() const
	{
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		return std::make_pair(glfwExtensions, glfwExtensionCount);
	}

	VkResult Window::CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const
	{
		return glfwCreateWindowSurface(instance, m_window, allocator, surface);
	}
}
