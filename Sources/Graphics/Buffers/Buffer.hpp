#pragma once

#include <vulkan/vulkan.h>
#include "Graphics/Descriptors/DescriptorSet.hpp"

namespace acid {
/**
 * @brief Interface that represents a buffer.
 */
class ACID_EXPORT Buffer {
public:
	enum class Status {
		Reset, Changed, Normal
	};

	/**
	 * Creates a new buffer with optional data.
	 * @param size Size of the buffer in bytes.
	 * @param usage Usage flag bitmask for the buffer (i.e. index, vertex, uniform buffer).
	 * @param properties Memory properties for this buffer (i.e. device local, host visible, coherent).
	 * @param data Pointer to the data that should be copied to the buffer after creation (optional, if not set, no data is copied over).
	 */
	Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, const void *data = nullptr);

	virtual ~Buffer();

	void MapMemory(void **data) const;
	void UnmapMemory() const;

	const VkDeviceSize &GetSize() const { return m_size; }
	const VkBuffer &GetBuffer() const { return m_buffer; }
	const VkDeviceMemory &GetBufferMemory() const { return m_bufferMemory; }

	static uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags &requiredProperties);

	static void InsertBufferMemoryBarrier(const CommandBuffer &commandBuffer, const VkBuffer &buffer, const VkAccessFlags &srcAccessMask, const VkAccessFlags &dstAccessMask,
		const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask, const VkDeviceSize &offset = 0, const VkDeviceSize & size = VK_WHOLE_SIZE);

protected:
	VkDeviceSize m_size;
	VkBuffer m_buffer = VK_NULL_HANDLE;
	VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;
};
}
