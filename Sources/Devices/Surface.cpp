#include "Surface.hpp"

#include "Graphics/Graphics.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Window.hpp"

namespace acid {
Surface::Surface(const Instance *instance, const PhysicalDevice *physicalDevice) :
	instance(instance),
	physicalDevice(physicalDevice) {
	// Creates the surface.
	Graphics::CheckVk(Window::Get()->CreateSurface(*instance, nullptr, &surface));

	Graphics::CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*physicalDevice, surface, &capabilities));

	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &surfaceFormatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

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
}

Surface::~Surface() {
	vkDestroySurfaceKHR(*instance, surface, nullptr);
}
}
