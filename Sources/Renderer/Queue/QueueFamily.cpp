#include "QueueFamily.hpp"

namespace fl
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

		for (auto queueFamily : queueFamilies)
		{
			if (queueFamily.queueCount <= 0)
			{
				continue;
			}

			// Check for graphics and presentation support.
			if (queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT)
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

				if (presentSupport)
				{
					indices.graphicsFamily = i;
				}
			}

			// Check for transfer support.
			if (queueFamily.queueFlags && VK_QUEUE_TRANSFER_BIT)
			{
				indices.presentFamily = i;
			}

			// Check for compute support.
			if (queueFamily.queueFlags && VK_QUEUE_COMPUTE_BIT)
			{
				indices.computeFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}

			i++;
		}

		indices.array = { static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.presentFamily), static_cast<uint32_t>(indices.computeFamily) };
		return indices;
	}
}
