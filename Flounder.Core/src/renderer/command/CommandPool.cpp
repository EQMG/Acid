#include "CommandPool.hpp"
#include "../queue/QueueFamily.hpp"

namespace Flounder
{
	CommandPool::CommandPool() :
		m_commandPool(VK_NULL_HANDLE)
	{
	}

	CommandPool::~CommandPool()
	{
	}

	void CommandPool::Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
	{
		m_commandPool = VkCommandPool();

		QueueFamilyIndices queueFamilyIndices = QueueFamily::FindQueueFamilies(physicalDevice, surface);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndex;
		poolInfo.flags = flags;

		GlfwVulkan::ErrorVk(vkCreateCommandPool(*logicalDevice, &poolInfo, nullptr, &m_commandPool));
	}

	void CommandPool::Cleanup(const VkDevice *logicalDevice)
	{
		vkDestroyCommandPool(*logicalDevice, m_commandPool, nullptr);
	}
}