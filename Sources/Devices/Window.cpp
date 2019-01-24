#include "Window.hpp"

#include <cassert>
#include <algorithm>
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
		if (event == GLFW_CONNECTED)
		{
			Log::Error("Monitor connected: %s\n", glfwGetMonitorName(monitor));
			Window::Get()->m_monitors.emplace_back(Monitor(monitor));
			Window::Get()->m_onMonitorConnect(static_cast<uint32_t>(Window::Get()->m_monitors.size() - 1), true);
		}
		else if (event == GLFW_DISCONNECTED)
		{
			Log::Error("Monitor disconnected: %s\n", glfwGetMonitorName(monitor));
			auto &monitors = Window::Get()->m_monitors;
			monitors.erase(std::remove_if(monitors.begin(), monitors.end(), [&](Monitor &m) {
				return monitor == m.GetMonitor();
			}), monitors.end());

			for (auto &m : monitors)
			{
				m.SetPrimary(m.GetMonitor() == glfwGetPrimaryMonitor());
			}

			Window::Get()->m_onMonitorConnect(0, false);
		}
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

	void CallbackClose(GLFWwindow *window)
	{
		Window::Get()->m_closed = false;
		Engine::Get()->RequestClose(false);
		Window::Get()->m_onClose();
	}

	void CallbackFocus(GLFWwindow *window, int32_t focused)
	{
		Window::Get()->m_focused = static_cast<bool>(focused);
	}

	void CallbackIconify(GLFWwindow *window, int32_t iconified)
	{
		Window::Get()->m_iconified = iconified == GLFW_TRUE;
		Window::Get()->m_onIconify(iconified == GLFW_TRUE);
	}

	void CallbackFrame(GLFWwindow *window, int32_t width, int32_t height)
	{
		Window::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
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
		m_borderless(false),
		m_resizable(true),
		m_floating(false),
		m_fullscreen(false),
		m_closed(false),
		m_focused(true),
		m_iconified(false),
		m_window(nullptr),
		m_monitors(std::vector<Monitor>()),
		m_onMonitorConnect(Delegate<void(uint32_t, bool)>()),
		m_onClose(Delegate<void()>()),
		m_onIconify(Delegate<void(bool)>())
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

		// Get connected monitors.
		int32_t monitorCount;
		GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

		for (uint32_t i = 0; i < monitorCount; i++)
		{
			m_monitors.emplace_back(Monitor(monitors[i]));
		}

		auto videoMode = m_monitors[0].GetVideoMode();

		// Create a windowed mode window and its context.
		m_window = glfwCreateWindow(m_fullscreen ? m_fullscreenWidth : m_windowWidth, m_fullscreen ? m_fullscreenHeight : m_windowHeight, m_title.c_str(), nullptr, nullptr);

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
		m_positionX = (videoMode.m_width - m_windowWidth) / 2;
		m_positionY = (videoMode.m_height - m_windowHeight) / 2;
		glfwSetWindowPos(m_window, m_positionX, m_positionY);

		// Sets fullscreen if enabled.
		if (m_fullscreen)
		{
			SetFullscreen(true);
		}

		// Shows the glfw window.
		glfwShowWindow(m_window);

		// Sets the displays callbacks.
		glfwSetWindowPosCallback(m_window, CallbackPosition);
		glfwSetWindowSizeCallback(m_window, CallbackSize);
		glfwSetWindowCloseCallback(m_window, CallbackClose);
		glfwSetWindowFocusCallback(m_window, CallbackFocus);
		glfwSetWindowIconifyCallback(m_window, CallbackIconify);
		glfwSetFramebufferSizeCallback(m_window, CallbackFrame);
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

	void Window::SetIcons(const std::vector<std::string> &filenames)
	{
		std::vector<GLFWimage> icons = {};

		for (const auto &filename : filenames)
		{
			uint32_t width = 0;
			uint32_t height = 0;
			uint32_t components = 0;
			uint8_t *data = Texture::LoadPixels(filename, &width, &height, &components);

			if (data == nullptr)
			{
				continue;
			}

			GLFWimage icon = {};
			icon.width = width;
			icon.height = height;
			icon.pixels = data;
			icons.emplace_back(icon);
		}

		glfwSetWindowIcon(m_window, icons.size(), icons.data());

		for (const auto &icon : icons)
		{
			Texture::DeletePixels(icon.pixels);
		}
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

	void Window::SetFullscreen(const bool &fullscreen, const std::optional<Monitor> &monitor)
	{
		m_fullscreen = fullscreen;

		auto selected = monitor ? *monitor : m_monitors[0];
		auto videoMode = selected.GetVideoMode();

		if (fullscreen)
		{
#if defined(ACID_VERBOSE)
			printf("Window is going fullscreen\n");
#endif
			m_fullscreenWidth = static_cast<uint32_t>(videoMode.m_width);
			m_fullscreenHeight = static_cast<uint32_t>(videoMode.m_height);
			glfwSetWindowMonitor(m_window, selected.GetMonitor(), 0, 0, m_fullscreenWidth, m_fullscreenHeight, GLFW_DONT_CARE);
		}
		else
		{
#if defined(ACID_VERBOSE)
			printf("Window is going windowed\n");
#endif
			m_positionX = (videoMode.m_width - m_windowWidth) / 2;
			m_positionY = (videoMode.m_height - m_windowHeight) / 2;
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
