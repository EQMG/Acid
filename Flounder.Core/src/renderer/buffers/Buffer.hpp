#pragma once

#include "../../platforms/glfw/Platform.hpp"

namespace Flounder
{
	class Buffer
	{
	protected:
		VkDeviceSize m_size;
		VkBuffer m_buffer;
		VkDeviceMemory m_bufferMemory;
	public:
		Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties);

		~Buffer();

		VkDeviceSize GetSize() { return m_size; }

		VkBuffer GetBuffer() { return m_buffer; }

		VkDeviceMemory GetBufferMemory() { return m_bufferMemory; }

		static uint32_t FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties);

		static void CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size);
	};
}
