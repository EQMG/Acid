#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/DescriptorSet.hpp"

namespace acid
{
	class ACID_EXPORT Buffer
	{
	protected:
		VkDeviceSize m_size;
		VkBuffer m_buffer;
		VkDeviceMemory m_bufferMemory;
	public:
		Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties);

		virtual ~Buffer();

		VkDeviceSize GetSize() const { return m_size; }

		VkBuffer GetBuffer() const { return m_buffer; }

		VkDeviceMemory GetBufferMemory() const { return m_bufferMemory; }

		static uint32_t FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties);

		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, const VkDeviceSize &size);
	};
}
