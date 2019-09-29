#include "Window.hpp"

#include <GLFW/glfw3.h>
#include "Bitmaps/Bitmap.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
void CallbackError(int32_t error, const char *description) {
	Window::CheckGlfw(error);
	Log::Error("GLFW error: ", description, ", ", error, '\n');
}

void CallbackMonitor(GLFWmonitor *monitor, int32_t event) {
	auto &monitors = Window::Get()->m_monitors;

	if (event == GLFW_CONNECTED) {
		auto &it = monitors.emplace_back(std::make_unique<Monitor>(monitor));
		Window::Get()->m_onMonitorConnect(it.get(), true);
	} else if (event == GLFW_DISCONNECTED) {
		for (auto &m : monitors) {
			if (m->GetMonitor() == monitor) {
				Window::Get()->m_onMonitorConnect(m.get(), false);
			}
		}

		monitors.erase(std::remove_if(monitors.begin(), monitors.end(), [monitor](std::unique_ptr<Monitor> &m) {
			return monitor == m->GetMonitor();
		}));
	}
}

void CallbackWindowPosition(GLFWwindow *window, int32_t xpos, int32_t ypos) {
	if (Window::Get()->m_fullscreen) 
		return;
	Window::Get()->m_position = {xpos, ypos};
	Window::Get()->m_onPosition(Window::Get()->m_position);
}

void CallbackWindowSize(GLFWwindow *window, int32_t width, int32_t height) {
	if (width <= 0 || height <= 0) {
		return;
	}

	if (Window::Get()->m_fullscreen) {
		Window::Get()->m_fullscreenSize = {width, height};
		Window::Get()->m_onSize(Window::Get()->m_fullscreenSize);
	} else {
		Window::Get()->m_size = {width, height};
		Window::Get()->m_onSize(Window::Get()->m_size);
	}
}

void CallbackWindowClose(GLFWwindow *window) {
	Window::Get()->m_closed = false;
	Engine::Get()->RequestClose();
	Window::Get()->m_onClose();
}

void CallbackWindowFocus(GLFWwindow *window, int32_t focused) {
	Window::Get()->m_focused = static_cast<bool>(focused);
	Window::Get()->m_onFocus(focused == GLFW_TRUE);
}

void CallbackWindowIconify(GLFWwindow *window, int32_t iconified) {
	Window::Get()->m_iconified = iconified;
	Window::Get()->m_onIconify(iconified);
}

void CallbackFramebufferSize(GLFWwindow *window, int32_t width, int32_t height) {
	if (Window::Get()->m_fullscreen)
		Window::Get()->m_fullscreenSize = {width, height};
	else
		Window::Get()->m_size = {width, height};
	Graphics::Get()->SetFramebufferResized();
}

Window::Window() :
	m_size(1080, 720),
	m_title("Acid Window"),
	m_resizable(true),
	m_focused(true) {
	// Set the error error callback
	glfwSetErrorCallback(CallbackError);

	// Initialize the GLFW library.
	if (glfwInit() == GLFW_FALSE) {
		throw std::runtime_error("GLFW failed to initialize");
	}

	// Checks Vulkan support on GLFW.
	if (glfwVulkanSupported() == GLFW_FALSE) {
		throw std::runtime_error("GLFW failed to find Vulkan support");
	}

	// Set the monitor callback
	glfwSetMonitorCallback(CallbackMonitor);

	// The window will stay hidden until after creation.
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	// Disable context creation.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// Fixes 16 bit stencil bits in macOS.
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	// No stereo view!
	glfwWindowHint(GLFW_STEREO, GLFW_FALSE);

	// Get connected monitors.
	int32_t monitorCount;
	auto monitors = glfwGetMonitors(&monitorCount);

	for (uint32_t i = 0; i < static_cast<uint32_t>(monitorCount); i++) {
		m_monitors.emplace_back(std::make_unique<Monitor>(monitors[i]));
	}

	auto videoMode = m_monitors[0]->GetVideoMode();

	// Create a windowed mode window and its context.
	m_window = glfwCreateWindow(m_size.m_x, m_size.m_y, m_title.c_str(), nullptr, nullptr);

	// Gets any window errors.
	if (!m_window) {
		glfwTerminate();
		throw std::runtime_error("GLFW failed to create the window");
	}

	// Sets the user pointer.
	glfwSetWindowUserPointer(m_window, this);

	// Window attributes that can change later.
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, !m_borderless);
	glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable);
	glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating);

	// Centre the window position.
	m_position.m_x = (videoMode.m_width - m_size.m_x) / 2;
	m_position.m_y = (videoMode.m_height - m_size.m_y) / 2;
	glfwSetWindowPos(m_window, m_position.m_x, m_position.m_y);

	// Sets fullscreen if enabled.
	if (m_fullscreen) {
		SetFullscreen(true);
	}

	// Shows the glfw window.
	glfwShowWindow(m_window);

	// Sets the displays callbacks.
	glfwSetWindowPosCallback(m_window, CallbackWindowPosition);
	glfwSetWindowSizeCallback(m_window, CallbackWindowSize);
	glfwSetWindowCloseCallback(m_window, CallbackWindowClose);
	glfwSetWindowFocusCallback(m_window, CallbackWindowFocus);
	glfwSetWindowIconifyCallback(m_window, CallbackWindowIconify);
	glfwSetFramebufferSizeCallback(m_window, CallbackFramebufferSize);
}

Window::~Window() {
	// Free the window callbacks and destroy the window.
	glfwDestroyWindow(m_window);

	// Terminate GLFW.
	glfwTerminate();

	m_closed = true;
}

void Window::Update() {
	// Polls for window events.
	glfwPollEvents();
}

void Window::SetSize(const Vector2i &size) {
	m_size.m_x = size.m_x == -1 ? m_size.m_x : size.m_x;
	m_size.m_y = size.m_y == -1 ? m_size.m_y : size.m_y;
	glfwSetWindowSize(m_window, m_size.m_x, m_size.m_y);
}

void Window::SetPosition(const Vector2i &position) {
	m_position.m_x = position.m_x == -1 ? m_position.m_x : position.m_x;
	m_position.m_y = position.m_y == -1 ? m_position.m_y : position.m_y;
	glfwSetWindowPos(m_window, m_position.m_x, m_position.m_y);
}

void Window::SetTitle(const std::string &title) {
	m_title = title;
	glfwSetWindowTitle(m_window, m_title.c_str());
	m_onTitle(m_title);
}

void Window::SetIcons(const std::vector<std::filesystem::path> &filenames) {
	std::vector<std::unique_ptr<Bitmap>> bitmaps;
	std::vector<GLFWimage> icons;

	for (const auto &filename : filenames) {
		auto bitmap = std::make_unique<Bitmap>(filename);

		if (!bitmap.get()) continue;

		GLFWimage icon = {};
		icon.width = bitmap->GetSize().m_x;
		icon.height = bitmap->GetSize().m_y;
		icon.pixels = bitmap->GetData().get();
		icons.emplace_back(icon);
		bitmaps.emplace_back(std::move(bitmap));
	}

	glfwSetWindowIcon(m_window, static_cast<int32_t>(icons.size()), icons.data());
}

void Window::SetBorderless(bool borderless) {
	m_borderless = borderless;
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, !m_borderless);
	m_onBorderless(m_borderless);
}

void Window::SetResizable(bool resizable) {
	m_resizable = resizable;
	glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable);
	m_onResizable(m_resizable);
}

void Window::SetFloating(bool floating) {
	m_floating = floating;
	glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating);
	m_onFloating(m_floating);
}

void Window::SetFullscreen(bool fullscreen, Monitor *monitor) {
	auto selected = monitor ? monitor : GetCurrentMonitor();
	auto videoMode = selected->GetVideoMode();

	m_fullscreen = fullscreen;

	if (m_fullscreen) {
#if defined(ACID_DEBUG)
		printf("Window is going fullscreen\n");
#endif
		m_fullscreenSize = {videoMode.m_width, videoMode.m_height};
		glfwSetWindowMonitor(m_window, selected->GetMonitor(), 0, 0, m_fullscreenSize.m_x, m_fullscreenSize.m_y, GLFW_DONT_CARE);
	} else {
#if defined(ACID_DEBUG)
		printf("Window is going windowed\n");
#endif
		m_position = ((Vector2i(videoMode.m_width, videoMode.m_height) - m_size) / 2) + selected->GetPosition();
		glfwSetWindowMonitor(m_window, nullptr, m_position.m_x, m_position.m_y, m_size.m_x, m_size.m_y, GLFW_DONT_CARE);
	}

	m_onFullscreen(m_fullscreen);
}

void Window::SetIconified(bool iconify) {
	if (!m_iconified && iconify) {
		glfwIconifyWindow(m_window);
	} else if (m_iconified && !iconify) {
		glfwRestoreWindow(m_window);
	}
}

const Monitor *Window::GetPrimaryMonitor() const {
	for (const auto &monitor : m_monitors) {
		if (monitor->IsPrimary())
			return monitor.get();
	}
	return nullptr;
}

int32_t OverlappingArea(Vector2i l1, Vector2i r1, Vector2i l2, Vector2i r2) {
	int area1 = abs(l1.m_x - r1.m_x) *
		abs(l1.m_y - r1.m_y);

	int area2 = abs(l2.m_x - r2.m_x) *
		abs(l2.m_y - r2.m_y);

	int areaI = (std::min(r1.m_x, r2.m_x) -
		std::max(l1.m_x, l2.m_x)) *
		(std::min(r1.m_y, r2.m_y) -
			std::max(l1.m_y, l2.m_y));

	return (area1 + area2 - areaI);
}

const Monitor *Window::GetCurrentMonitor() const {
	if (m_fullscreen) {
		auto glfwMonitor = glfwGetWindowMonitor(m_window);
		for (const auto &monitor : m_monitors) {
			if (monitor->m_monitor == glfwMonitor)
				return monitor.get();
		}
		return nullptr;
	}
	std::multimap<int32_t, const Monitor *> rankedMonitor;

	for (const auto &monitor : m_monitors) {
		rankedMonitor.emplace(OverlappingArea(monitor->GetWorkareaPosition(), monitor->GetWorkareaPosition() + monitor->GetWorkareaSize(), m_position, m_position + m_size), monitor.get());
	}

	if (rankedMonitor.begin()->first > 0) {
		return rankedMonitor.begin()->second;
	}

	return nullptr;
}

std::string Window::StringifyResultGlfw(int32_t result) {
	switch (result) {
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

void Window::CheckGlfw(int32_t result) {
	if (result) {
		return;
	}

	auto failure = StringifyResultGlfw(result);
	Log::Error("GLFW error: ", failure, ", ", result, '\n');
	throw std::runtime_error("GLFW error: " + failure);
}

std::pair<const char **, uint32_t> Window::GetInstanceExtensions() const {
	uint32_t glfwExtensionCount;
	auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	return std::make_pair(glfwExtensions, glfwExtensionCount);
}

VkResult Window::CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const {
	return glfwCreateWindowSurface(instance, m_window, allocator, surface);
}
}
