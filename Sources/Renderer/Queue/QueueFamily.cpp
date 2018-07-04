#include "QueueFamily.hpp"

namespace fl
{
	QueueFamilyIndices QueueFamily::FindQueueFamilies(const VkSurfaceKHR &surface)
	{
		auto physicalDevice = Display::Get()->GetVkPhysicalDevice();

		int graphicsFamily = -1;
		int computeFamily = -1;
		int transferFamily = -1;
		int sparseFamily = -1;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Iterate through queue families to find one that supports VK_QUEUE_GRAPHICS_BIT.
		int i = 0;

		for (auto &queueFamily : queueFamilies)
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
					graphicsFamily = i;
				}
			}

			// Check for compute support.
			if (queueFamily.queueFlags && VK_QUEUE_COMPUTE_BIT)
			{
				computeFamily = i;
			}

			// Check for transfer support.
			if (queueFamily.queueFlags && VK_QUEUE_TRANSFER_BIT)
			{
				transferFamily = i;
			}

			// Check for sparse binding support.
			if (queueFamily.queueFlags && VK_QUEUE_SPARSE_BINDING_BIT)
			{
				sparseFamily = i;
			}

			if (graphicsFamily >= 0 && computeFamily >= 0 && transferFamily >= 0 && sparseFamily >= 0)
			{
				break;
			}

			i++;
		}

		return QueueFamilyIndices(graphicsFamily, computeFamily, transferFamily, sparseFamily);
	}
}
