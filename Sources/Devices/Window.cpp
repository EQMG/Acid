#include "Window.hpp"

#include <GLFW/glfw3.h>
#include "Graphics/Graphics.hpp"
#include "Graphics/Images/Image.hpp"

namespace acid
{
void CallbackError(int32_t error, const char *description)
{
	Window::CheckGlfw(error);
	Log::Error("GLFW error: %s, %i\n", description, error);
}

void CallbackMonitor(GLFWmonitor *monitor, int32_t event)
{
	auto &monitors = Window::Get()->m_monitors;

	if (event == GLFW_CONNECTED)
	{
		auto &it = monitors.emplace_back(std::make_unique<Monitor>(monitor));
		Window::Get()->m_onMonitorConnect(it.get(), true);
	}
	else if (event == GLFW_DISCONNECTED)
	{
		for (auto &m : monitors)
		{
			m->SetPrimary(m->GetMonitor() == glfwGetPrimaryMonitor());

			if (m->GetMonitor() == monitor)
			{
				Window::Get()->m_onMonitorConnect(m.get(), false);
			}
		}

		monitors.erase(std::remove_if(monitors.begin(), monitors.end(), [monitor](std::unique_ptr<Monitor> &m)
		{
			return monitor == m->GetMonitor();
		}));
	}
}

void CallbackPosition(GLFWwindow *window, int32_t xpos, int32_t ypos)
{
	if (!Window::Get()->m_fullscreen)
	{
		Window::Get()->m_position.m_x = static_cast<uint32_t>(xpos);
		Window::Get()->m_position.m_y = static_cast<uint32_t>(ypos);
	}

	Window::Get()->m_onPosition(Window::Get()->m_position);
}

void CallbackSize(GLFWwindow *window, int32_t width, int32_t height)
{
	if (width <= 0 || height <= 0)
	{
		return;
	}

	if (Window::Get()->m_fullscreen)
	{
		Window::Get()->m_fullscreenSize.m_x = static_cast<uint32_t>(width);
		Window::Get()->m_fullscreenSize.m_y = static_cast<uint32_t>(height);
	}
	else
	{
		Window::Get()->m_size.m_x = static_cast<uint32_t>(width);
		Window::Get()->m_size.m_y = static_cast<uint32_t>(height);
	}

	Window::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	Graphics::Get()->UpdateSurfaceCapabilities();
	Window::Get()->m_onSize(Window::Get()->m_size);
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
	Window::Get()->m_onFocus(focused == GLFW_TRUE);
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
	m_size(1080, 720),
	m_fullscreenSize(0, 0),
	m_aspectRatio(1.5f),
	m_position(0, 0),
	m_title("Acid Loading..."),
	m_borderless(false),
	m_resizable(true),
	m_floating(false),
	m_fullscreen(false),
	m_closed(false),
	m_focused(true),
	m_iconified(false),
	m_window(nullptr)
{
	// Set the error error callback
	glfwSetErrorCallback(CallbackError);

	// Initialize the GLFW library.
	if (glfwInit() == GLFW_FALSE)
	{
		throw std::runtime_error("GLFW failed to initialize");
	}

	// Checks Vulkan support on GLFW.
	if (glfwVulkanSupported() == GLFW_FALSE)
	{
		throw std::runtime_error("GLFW failed to find Vulkan support");
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

	for (uint32_t i = 0; i < static_cast<uint32_t>(monitorCount); i++)
	{
		m_monitors.emplace_back(std::make_unique<Monitor>(monitors[i]));
	}

	auto videoMode = m_monitors[0]->GetVideoMode();

	// Create a windowed mode window and its context.
	m_window = glfwCreateWindow(m_fullscreen ? m_fullscreenSize.m_x : m_size.m_x, m_fullscreen ? m_fullscreenSize.m_y : m_size.m_y, m_title.c_str(), nullptr, nullptr);

	// Gets any window errors.
	if (m_window == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("GLFW failed to create the window");
	}

	// Sets the user pointer.
	glfwSetWindowUserPointer(m_window, this);

	// Window attributes that can change later.
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, m_borderless ? GLFW_FALSE : GLFW_TRUE);
	glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating ? GLFW_TRUE : GLFW_FALSE);

	// Centre the window position.
	m_position.m_x = (videoMode.m_width - m_size.m_x) / 2;
	m_position.m_y = (videoMode.m_height - m_size.m_y) / 2;
	glfwSetWindowPos(m_window, m_position.m_x, m_position.m_y);

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

void Window::SetSize(const Vector2i &size)
{
	m_size.m_x = size.m_x == -1 ? m_size.m_x : size.m_x;
	m_size.m_y = size.m_y == -1 ? m_size.m_y : size.m_y;
	m_aspectRatio = static_cast<float>(m_size.m_x) / static_cast<float>(m_size.m_y);
	glfwSetWindowSize(m_window, m_size.m_x, m_size.m_y);
}

void Window::SetPosition(const Vector2i &position)
{
	m_position.m_x = position.m_x == -1 ? m_position.m_x : position.m_x;
	m_position.m_y = position.m_y == -1 ? m_position.m_y : position.m_y;
	glfwSetWindowPos(m_window, m_position.m_x, m_position.m_y);
}

void Window::SetTitle(const std::string &title)
{
	m_title = title;
	glfwSetWindowTitle(m_window, m_title.c_str());
	m_onTitle(m_title);
}

void Window::SetIcons(const std::vector<std::string> &filenames)
{
	std::vector<GLFWimage> icons;
	std::vector<std::unique_ptr<uint8_t[]>> pixels;

	for (const auto &filename : filenames)
	{
		Vector2ui extent;
		uint32_t components;
		VkFormat format;
		auto data = Image::LoadPixels(filename, extent, components, format);

		if (data == nullptr)
		{
			continue;
		}

		GLFWimage icon = {};
		icon.width = extent.m_x;
		icon.height = extent.m_y;
		icon.pixels = data.get();
		icons.emplace_back(icon);
		pixels.emplace_back(std::move(data));
	}

	glfwSetWindowIcon(m_window, static_cast<int32_t>(icons.size()), icons.data());
}

void Window::SetBorderless(const bool &borderless)
{
	m_borderless = borderless;
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, m_borderless ? GLFW_FALSE : GLFW_TRUE);
	m_onBorderless(m_borderless);
}

void Window::SetResizable(const bool &resizable)
{
	m_resizable = resizable;
	glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
	m_onResizable(m_resizable);
}

void Window::SetFloating(const bool &floating)
{
	m_floating = floating;
	glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating ? GLFW_TRUE : GLFW_FALSE);
	m_onFloating(m_floating);
}

void Window::SetFullscreen(const bool &fullscreen, Monitor *monitor)
{
	m_fullscreen = fullscreen;

	auto selected = monitor != nullptr ? monitor : m_monitors[0].get();
	auto videoMode = selected->GetVideoMode();

	if (fullscreen)
	{
#if defined(ACID_VERBOSE)
		printf("Window is going fullscreen\n");
#endif
		m_fullscreenSize.m_x = static_cast<uint32_t>(videoMode.m_width);
		m_fullscreenSize.m_y = static_cast<uint32_t>(videoMode.m_height);
		glfwSetWindowMonitor(m_window, selected->GetMonitor(), 0, 0, m_fullscreenSize.m_x, m_fullscreenSize.m_y, GLFW_DONT_CARE);
	}
	else
	{
#if defined(ACID_VERBOSE)
		printf("Window is going windowed\n");
#endif
		m_position.m_x = (videoMode.m_width - m_size.m_x) / 2;
		m_position.m_y = (videoMode.m_height - m_size.m_y) / 2;
		glfwSetWindowMonitor(m_window, nullptr, m_position.m_x, m_position.m_y, m_size.m_x, m_size.m_y, GLFW_DONT_CARE);
	}

	m_onFullscreen(m_fullscreen);
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
	throw std::runtime_error("GLFW error: " + result);
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
