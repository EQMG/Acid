#pragma once

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
	Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const void *data = nullptr);

	virtual ~Buffer();

	void MapMemory(void **data) const;
	void UnmapMemory() const;

	VkDeviceSize GetSize() const { return size; }
	const VkBuffer &GetBuffer() const { return buffer; }
	const VkDeviceMemory &GetBufferMemory() const { return bufferMemory; }

	static uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags &requiredProperties);

	static void InsertBufferMemoryBarrier(const CommandBuffer &commandBuffer, const VkBuffer &buffer, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);

protected:
	VkDeviceSize size;
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
};
}
