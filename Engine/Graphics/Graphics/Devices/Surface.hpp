#pragma once

#include <volk.h>

#include "GraphicsExport.hpp"

namespace acid {
class Instance;
class LogicalDevice;
class PhysicalDevice;
class Window;

class ACID_GRAPHICS_EXPORT Surface {
	friend class Graphics;
public:
	Surface(const Instance &instance, const PhysicalDevice &physicalDevice, const LogicalDevice &logicalDevice, const Window &window);
	~Surface();

	operator const VkSurfaceKHR &() const { return surface; }

	const VkSurfaceKHR &GetSurface() const { return surface; }
	const VkSurfaceCapabilitiesKHR &GetCapabilities() const { return capabilities; }
	const VkSurfaceFormatKHR &GetFormat() const { return format; }

private:
	const Instance &instance;
	const PhysicalDevice &physicalDevice;
	const LogicalDevice &logicalDevice;
	const Window &window;

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkSurfaceCapabilitiesKHR capabilities = {};
	VkSurfaceFormatKHR format = {};
};
}
