#pragma once

#include "../../platforms/glfw/GlfwVulkan.hpp"

#include "Vertex.hpp"

namespace Flounder
{
	class Buffer
	{
	protected:
		VkBuffer m_buffer;
		VkDeviceMemory m_bufferMemory;
	public:
		Buffer();

		~Buffer();

		void Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties);

		void Cleanup(const VkDevice &logicalDevice);

		VkBuffer GetBuffer() { return m_buffer; }

		VkDeviceMemory GetBufferMemory() { return m_bufferMemory; }

		static uint32_t FindMemoryType(const VkPhysicalDevice &physicalDevice, const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties);

		static void CopyBuffer(const VkDevice &logicalDevice, const VkQueue &queue, const VkCommandPool &transferCommandPool, const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size);
	};
}
