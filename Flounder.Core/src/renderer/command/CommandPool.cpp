#include "CommandPool.hpp"
#include "../queue/QueueFamily.hpp"

namespace Flounder
{
	CommandPool::CommandPool(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags) :
		m_logicalDevice(logicalDevice)
	{
		QueueFamilyIndices queueFamilyIndices = QueueFamily::FindQueueFamilies(physicalDevice, surface);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndex;
		poolInfo.flags = flags;

		if (vkCreateCommandPool(*logicalDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create command pool!");
		}
		else
		{
			printf("Command pool created successfully\n");
		}
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(*m_logicalDevice, m_commandPool, nullptr);
	}
}
