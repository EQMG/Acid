#pragma once

#include "../../platforms/glfw/GlfwVulkan.hpp"

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

		void Create(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties);

		void Cleanup();

		VkBuffer GetBuffer() { return m_buffer; }

		VkDeviceMemory GetBufferMemory() { return m_bufferMemory; }

		static uint32_t FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties);

		static void CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size);
	};
}
