#pragma once

#include <stdexcept>

#include "../../platforms/glfw/GlfwVulkan.h"

namespace Flounder
{
	class CommandPool
	{
	private:
		const VkDevice *m_logicalDevice;
		VkCommandPool m_commandPool;
	public:
		CommandPool(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);

		~CommandPool();

		VkCommandPool GetCommandPool() const { return m_commandPool; }
	};
}
