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
		CommandPool();

		~CommandPool();

		void Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);

		void Cleanup();

		VkCommandPool *GetCommandPool() { return &m_commandPool; }
	};
}
