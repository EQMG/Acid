#include "QueueIndices.hpp"

#include <vector>

namespace acid
{
	QueueIndices::QueueIndices(const int &graphicsFamily, const int &computeFamily, const int &transferFamily, const int &sparseFamily) :
		m_graphicsFamily(graphicsFamily),
		m_computeFamily(computeFamily),
		m_transferFamily(transferFamily),
		m_sparseFamily(sparseFamily),
		m_array({static_cast<uint32_t>(m_graphicsFamily), static_cast<uint32_t>(m_computeFamily), static_cast<uint32_t>(m_transferFamily), static_cast<uint32_t>(m_sparseFamily)})
	{
	}

	QueueIndices::QueueIndices(const VkPhysicalDevice &physicalDevice) :
		m_graphicsFamily(-1),
		m_computeFamily(-1),
		m_transferFamily(-1),
		m_sparseFamily(-1)
	{
		uint32_t deviceQueueFamilyPropertyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyPropertyCount, nullptr);
		std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

		for (uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++)
		{
			// Check for graphics support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_graphicsFamily = i;
			}

			// Check for compute support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				m_computeFamily = i;
			}

			// Check for transfer support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				m_transferFamily = i;
			}

			// Check for sparse binding support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
			{
				m_sparseFamily = i;
			}
		}

		m_array = {static_cast<uint32_t>(m_graphicsFamily), static_cast<uint32_t>(m_computeFamily), static_cast<uint32_t>(m_transferFamily), static_cast<uint32_t>(m_sparseFamily)};
	}

	QueueIndices::QueueIndices(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface) :
		m_graphicsFamily(-1),
		m_computeFamily(-1),
		m_transferFamily(-1),
		m_sparseFamily(-1)
	{
		uint32_t deviceQueueFamilyPropertyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyPropertyCount, nullptr);
		std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

		for (uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++)
		{
			if (deviceQueueFamilyProperties[i].queueCount <= 0)
			{
				continue;
			}

			// Check for graphics and presentation support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				VkBool32 presentSupport;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

				if (presentSupport)
				{
					m_graphicsFamily = i;
				}
			}

			// Check for compute support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				m_computeFamily = i;
			}

			// Check for transfer support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				m_transferFamily = i;
			}

			// Check for sparse binding support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
			{
				m_sparseFamily = i;
			}

			if (m_graphicsFamily >= 0 && m_computeFamily >= 0 && m_transferFamily >= 0 && m_sparseFamily >= 0)
			{
				break;
			}

			i++;
		}

		m_array = {static_cast<uint32_t>(m_graphicsFamily), static_cast<uint32_t>(m_computeFamily), static_cast<uint32_t>(m_transferFamily), static_cast<uint32_t>(m_sparseFamily)};
	}

	bool QueueIndices::IsComplete() const
	{
		return m_graphicsFamily >= 0 && m_computeFamily >= 0 && m_transferFamily >= 0 && m_sparseFamily >= 0;
	}
}
