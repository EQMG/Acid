#pragma once

#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	class CommandPool
	{
	private:
		VkCommandPool m_commandPool;
	public:
		CommandPool();

		~CommandPool();

		void Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);

		void Cleanup(const VkDevice *logicalDevice);

		VkCommandPool *GetCommandPool() { return &m_commandPool; }
	};
}