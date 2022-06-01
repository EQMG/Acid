#include "Windows.hpp"

#include <algorithm>
#include <GLFW/glfw3.h>

namespace acid {
void CallbackError(int32_t error, const char *description) {
	Windows::CheckGlfw(error);
	Log::Error("GLFW error: ", description, ", ", error, '\n');
}

void CallbackMonitor(GLFWmonitor *glfwMonitor, int32_t event) {
	auto &monitors = Windows::Get()->monitors;

	if (event == GLFW_CONNECTED) {
		auto monitor = monitors.emplace_back(std::make_unique<Monitor>(glfwMonitor)).get();
		Windows::Get()->onMonitorConnect(monitor, true);
	} else if (event == GLFW_DISCONNECTED) {
		for (const auto &monitor : monitors) {
			if (monitor->GetMonitor() == glfwMonitor) {
				Windows::Get()->onMonitorConnect(monitor.get(), false);
			}
		}

		monitors.erase(std::remove_if(monitors.begin(), monitors.end(), [glfwMonitor](const auto &monitor) {
			return glfwMonitor == monitor->GetMonitor();
		}));
	}
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
	glfwTerminate();
}

void Windows::Update() {
	glfwPollEvents();
	for (auto &window : windows)
		window->Update();
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
