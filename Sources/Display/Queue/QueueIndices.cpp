#include "QueueIndices.hpp"

#include <vector>

namespace acid
{
	QueueIndices::QueueIndices(const int &graphicsFamily, const int &presentFamily, const int &computeFamily) :
		m_graphicsFamily(graphicsFamily),
		m_presentFamily(presentFamily),
		m_computeFamily(computeFamily),
		m_array(std::vector<uint32_t>())
	{
	}

	QueueIndices::QueueIndices(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface) :
		m_graphicsFamily(-1),
		m_presentFamily(-1),
		m_computeFamily(-1),
		m_array(std::vector<uint32_t>())
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

			// Check for presentation support.
			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

			if (deviceQueueFamilyProperties[i].queueCount > 0 && presentSupport)
			{
				m_presentFamily = i;
			}

			// Check for compute support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				m_computeFamily = i;
			}

			if (IsComplete())
			{
				break;
			}

			i++;
		}

		UpdateArray();
	}

	bool QueueIndices::IsComplete() const
	{
		return m_graphicsFamily >= 0 && m_presentFamily >= 0 && m_computeFamily >= 0;
	}

	void QueueIndices::UpdateArray()
	{
		if (m_graphicsFamily != -1)
		{
			m_array.emplace_back(m_graphicsFamily);
		}

		if (m_presentFamily != -1)
		{
			m_array.emplace_back(m_presentFamily);
		}

		if (m_computeFamily != -1)
		{
			m_array.emplace_back(m_computeFamily);
		}
	}
}
