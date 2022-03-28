#include "Windows.hpp"

#include <algorithm>
#include <GLFW/glfw3.h>

#include "Bitmaps/Bitmap.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
void CallbackError(int32_t error, const char *description) {
	Windows::CheckGlfw(error);
	Log::Error("GLFW error: ", description, ", ", error, '\n');
}

void CallbackMonitor(GLFWmonitor *monitor, int32_t event) {
	auto &monitors = Windows::Get()->monitors;

	if (event == GLFW_CONNECTED) {
		auto &it = monitors.emplace_back(std::make_unique<Monitor>(monitor));
		Windows::Get()->onMonitorConnect(it.get(), true);
	} else if (event == GLFW_DISCONNECTED) {
		for (auto &m : monitors) {
			if (m->GetMonitor() == monitor) {
				Windows::Get()->onMonitorConnect(m.get(), false);
			}
		}

		monitors.erase(std::remove_if(monitors.begin(), monitors.end(), [monitor](const auto &m) {
			return monitor == m->GetMonitor();
		}));
	}
}

void CallbackWindowPosition(GLFWwindow *window, int32_t xpos, int32_t ypos) {
	auto context = static_cast<Window *>(glfwGetWindowUserPointer(window));
	if (context->fullscreen) return;
	
	context->position = {xpos, ypos};
	context->onPosition(context->position);
}

void CallbackWindowSize(GLFWwindow *window, int32_t width, int32_t height) {
	if (width <= 0 || height <= 0) return;
	auto context = static_cast<Window *>(glfwGetWindowUserPointer(window));

	if (context->fullscreen) {
		context->fullscreenSize = {width, height};
		context->onSize(context->fullscreenSize);
	} else {
		context->size = {width, height};
		context->onSize(context->size);
	}
}

void CallbackWindowClose(GLFWwindow *window) {
	auto context = static_cast<Window *>(glfwGetWindowUserPointer(window));
	context->closed = false;
	//Engine::Get()->RequestClose();
	context->onClose();
}

void CallbackWindowFocus(GLFWwindow *window, int32_t focused) {
	auto context = static_cast<Window *>(glfwGetWindowUserPointer(window));
	context->focused = static_cast<bool>(focused);
	context->onFocus(focused == GLFW_TRUE);
}

void CallbackWindowIconify(GLFWwindow *window, int32_t iconified) {
	auto context = static_cast<Window *>(glfwGetWindowUserPointer(window));
	context->iconified = iconified;
	context->onIconify(iconified);
}

void CallbackFramebufferSize(GLFWwindow *window, int32_t width, int32_t height) {
	auto context = static_cast<Window *>(glfwGetWindowUserPointer(window));
	if (context->fullscreen)
		context->fullscreenSize = {width, height};
	else
		context->size = {width, height};
	// TODO: only set this window to resized
	Graphics::Get()->SetFramebufferResized(context->id);
}

Window::Window(std::size_t id) :
	id(id),
	size(1080, 720),
	title("Acid Window"),
	resizable(true),
	focused(true) {
	// Create a windowed mode window and its context.
	window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

	// Gets any window errors.
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("GLFW failed to create the window");
	}

	// Sets the user pointer.
	glfwSetWindowUserPointer(window, this);

	// Window attributes that can change later.
	glfwSetWindowAttrib(window, GLFW_DECORATED, !borderless);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, resizable);
	glfwSetWindowAttrib(window, GLFW_FLOATING, floating);

	// Centre the window position.
	//position.x = (videoMode.width - size.x) / 2;
	//position.y = (videoMode.height - size.y) / 2;
	//glfwSetWindowPos(window, position.x, position.y);

	// Sets fullscreen if enabled.
	if (fullscreen)
		SetFullscreen(true);

	// Shows the glfw window.
	glfwShowWindow(window);

	// Sets the displays callbacks.
	glfwSetWindowPosCallback(window, CallbackWindowPosition);
	glfwSetWindowSizeCallback(window, CallbackWindowSize);
	glfwSetWindowCloseCallback(window, CallbackWindowClose);
	glfwSetWindowFocusCallback(window, CallbackWindowFocus);
	glfwSetWindowIconifyCallback(window, CallbackWindowIconify);
	glfwSetFramebufferSizeCallback(window, CallbackFramebufferSize);
}

Window::~Window() {
	// Free the window callbacks and destroy the window.
	glfwDestroyWindow(window);

	closed = true;
}

void Window::SetSize(const Vector2i &size) {
	if (size.x != -1)
		this->size.x = size.x;
	if (size.y != -1)
		this->size.y = size.y;
	glfwSetWindowSize(window, size.x, size.y);
}

void Window::SetPosition(const Vector2i &position) {
	if (position.x != -1)
		this->position.x = position.x;
	if (position.x != -1)
		this->position.y = position.y;
	glfwSetWindowPos(window, position.x, position.y);
}

void Window::SetTitle(const std::string &title) {
	this->title = title;
	glfwSetWindowTitle(window, title.c_str());
	onTitle(title);
}

void Window::SetIcons(const std::vector<std::filesystem::path> &filenames) {
	std::vector<std::unique_ptr<Bitmap>> bitmaps;
	std::vector<GLFWimage> icons;

	for (const auto &filename : filenames) {
		auto bitmap = std::make_unique<Bitmap>(filename);

		if (!bitmap) continue;

		GLFWimage icon = {};
		icon.width = bitmap->GetSize().x;
		icon.height = bitmap->GetSize().y;
		icon.pixels = bitmap->GetData().get();
		icons.emplace_back(icon);
		bitmaps.emplace_back(std::move(bitmap));
	}

	glfwSetWindowIcon(window, static_cast<int32_t>(icons.size()), icons.data());
}

void Window::SetBorderless(bool borderless) {
	this->borderless = borderless;
	glfwSetWindowAttrib(window, GLFW_DECORATED, !borderless);
	onBorderless(borderless);
}

void Window::SetResizable(bool resizable) {
	this->resizable = resizable;
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, resizable);
	onResizable(resizable);
}

void Window::SetFloating(bool floating) {
	this->floating = floating;
	glfwSetWindowAttrib(window, GLFW_FLOATING, floating);
	onFloating(floating);
}

void Window::SetFullscreen(bool fullscreen, Monitor *monitor) {
	auto selected = monitor ? monitor : GetCurrentMonitor();
	auto videoMode = selected->GetVideoMode();

	this->fullscreen = fullscreen;

	if (fullscreen) {
#ifdef ACID_DEBUG
		Log::Out("Window is going fullscreen\n");
#endif
		fullscreenSize = {videoMode.width, videoMode.height};
		glfwSetWindowMonitor(window, selected->GetMonitor(), 0, 0, fullscreenSize.x, fullscreenSize.y, GLFW_DONT_CARE);
	} else {
#ifdef ACID_DEBUG
		Log::Out("Window is going windowed\n");
#endif
		position = ((Vector2i(videoMode.width, videoMode.height) - size) / 2) + selected->GetPosition();
		glfwSetWindowMonitor(window, nullptr, position.x, position.y, size.x, size.y, GLFW_DONT_CARE);
	}

	onFullscreen(fullscreen);
}

void Window::SetIconified(bool iconify) {
	if (!iconified && iconify) {
		glfwIconifyWindow(window);
	} else if (iconified && !iconify) {
		glfwRestoreWindow(window);
	}
}

int32_t OverlappingArea(Vector2i l1, Vector2i r1, Vector2i l2, Vector2i r2) {
	int area1 = std::abs(l1.x - r1.x) * std::abs(l1.y - r1.y);

	int area2 = std::abs(l2.x - r2.x) * std::abs(l2.y - r2.y);

	int areaI = (std::min(r1.x, r2.x) - std::max(l1.x, l2.x)) *
		(std::min(r1.y, r2.y) - std::max(l1.y, l2.y));

	return area1 + area2 - areaI;
}

const Monitor *Window::GetCurrentMonitor() const {
	if (fullscreen) {
		auto glfwMonitor = glfwGetWindowMonitor(window);
		for (const auto &monitor : Windows::Get()->GetMonitors()) {
			if (monitor->monitor == glfwMonitor)
				return monitor.get();
		}
		return nullptr;
	}
	std::multimap<int32_t, const Monitor *> rankedMonitor;
	auto where = rankedMonitor.end();

	for (const auto &monitor : Windows::Get()->GetMonitors()) {
		where = rankedMonitor.insert(where, {OverlappingArea(monitor->GetWorkareaPosition(), monitor->GetWorkareaPosition() + monitor->GetWorkareaSize(),
			position, position + size), monitor.get()});
	}

	if (rankedMonitor.begin()->first > 0)
		return rankedMonitor.begin()->second;

	return nullptr;
}

VkResult Window::CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const {
	return glfwCreateWindowSurface(instance, window, allocator, surface);
}

Windows::Windows() {
	// Set the error error callback
	glfwSetErrorCallback(CallbackError);

	// Initialize the GLFW library.
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("GLFW failed to initialize");

	// Checks Vulkan support on GLFW.
	if (glfwVulkanSupported() == GLFW_FALSE)
		throw std::runtime_error("GLFW failed to find Vulkan support");

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

	for (uint32_t i = 0; i < static_cast<uint32_t>(monitorCount); i++)
		this->monitors.emplace_back(std::make_unique<Monitor>(monitors[i]));
}

Windows::~Windows() {
	// Terminate GLFW.
	glfwTerminate();
}

void Windows::Update() {
	// Polls for window events.
	glfwPollEvents();
}

Window *Windows::AddWindow() {
	auto window = windows.emplace_back(std::make_unique<Window>(windows.size())).get();
	onAddWindow(window, true);
	return window;
}

const Window *Windows::GetWindow(WindowId id) const {
	if (id >= windows.size())
		return nullptr;
	return windows.at(id).get();
}

Window *Windows::GetWindow(WindowId id) {
	if (id >= windows.size())
		return nullptr;
	return windows.at(id).get();
}

const Monitor *Windows::GetPrimaryMonitor() const {
	for (const auto &monitor : monitors) {
		if (monitor->IsPrimary())
			return monitor.get();
	}
	return nullptr;
}

std::string Windows::StringifyResultGlfw(int32_t result) {
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

void Windows::CheckGlfw(int32_t result) {
	if (result) return;

	auto failure = StringifyResultGlfw(result);
	Log::Error("GLFW error: ", failure, ", ", result, '\n');
	throw std::runtime_error("GLFW error: " + failure);
}

std::pair<const char **, uint32_t> Windows::GetInstanceExtensions() const {
	uint32_t glfwExtensionCount;
	auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	return std::make_pair(glfwExtensions, glfwExtensionCount);
}
}
