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
			Reset,
			Changed,
			Normal
		};

		/// <summary>
		/// Creates a new buffer with optional data.
		/// </summary>
		///	<param name="size"> Size of the buffer in bytes </param>
		///	<param name="usage"> Usage flag bitmask for the buffer (i.e. index, vertex, uniform buffer) </param>
		///	<param name="properties"> Memory properties for this buffer (i.e. device local, host visible, coherent) </param>
		///	<param name="data"> Pointer to the data that should be copied to the buffer after creation (optional, if not set, no data is copied over) </param>
		Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, const void *data = nullptr);

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
