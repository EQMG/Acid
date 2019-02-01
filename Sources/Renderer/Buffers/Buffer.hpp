#pragma once

#include <cstring>
#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/DescriptorSet.hpp"

namespace acid
{
	class ACID_EXPORT Buffer
	{
	public:
		enum class Status
		{
			Reset, Changed, Normal
		};

		Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties);

		virtual ~Buffer();

		void CopyBuffer(void *data) const;

		const VkDeviceSize &GetSize() const { return m_size; }

		const VkBuffer &GetBuffer() const { return m_buffer; }

		const VkDeviceMemory &GetBufferMemory() const { return m_bufferMemory; }

		static uint32_t FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &requiredProperties);

		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, const VkDeviceSize &size);
	protected:
		VkDeviceSize m_size;
		VkBuffer m_buffer;
		VkDeviceMemory m_bufferMemory;
	};
}
