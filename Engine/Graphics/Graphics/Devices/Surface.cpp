#include "Surface.hpp"

#include "Devices/Windows.hpp"
#include "Graphics/Graphics.hpp"
#include "Instance.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

namespace acid {
Surface::Surface(const Instance &instance, const PhysicalDevice &physicalDevice, const LogicalDevice &logicalDevice, const Window &window) :
	instance(instance),
	physicalDevice(physicalDevice),
	logicalDevice(logicalDevice),
	window(window) {
	// Creates the surface.
	Graphics::CheckVk(window.CreateSurface(instance, nullptr, &surface));

	Graphics::CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

	if (surfaceFormatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		format.format = VK_FORMAT_B8G8R8A8_UNORM;
		format.colorSpace = surfaceFormats[0].colorSpace;
	} else {
		// Iterate over the list of available surface format and
		// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
		bool found_B8G8R8A8_UNORM = false;

		for (auto &surfaceFormat : surfaceFormats) {
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM) {
				format.format = surfaceFormat.format;
				format.colorSpace = surfaceFormat.colorSpace;
				found_B8G8R8A8_UNORM = true;
				break;
			}
		}

		// In case VK_FORMAT_B8G8R8A8_UNORM is not available
		// select the first available color format
		if (!found_B8G8R8A8_UNORM) {
			format.format = surfaceFormats[0].format;
			format.colorSpace = surfaceFormats[0].colorSpace;
		}
	}

	// Check for presentation support.
	VkBool32 presentSupport;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, logicalDevice.GetPresentFamily(), surface, &presentSupport);

	if (!presentSupport)
		throw std::runtime_error("Present queue family does not have presentation support");
}

Surface::~Surface() {
	vkDestroySurfaceKHR(instance, surface, nullptr);
}
}
