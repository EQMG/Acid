#include "QueueFamily.hpp"

#include <vector>
#include "../../Devices/Display.hpp"

namespace Flounder
{
	QueueFamilyIndices QueueFamily::FindQueueFamilies(const VkSurfaceKHR &surface)
	{
		const auto physicalDevice = Display::Get()->GetPhysicalDevice();

		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Iterate through queue families to find one that supports VK_QUEUE_GRAPHICS_BIT.
		int i = 0;

		for (const auto &queueFamily : queueFamilies)
		{
			// Check for graphics and presentation support.
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT)
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

				if (presentSupport)
				{
					indices.graphicsFamily = i;
				}
			}

			// Check for transfer support.
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags && VK_QUEUE_TRANSFER_BIT)
			{
				indices.transferFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}
}
