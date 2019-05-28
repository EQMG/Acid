#pragma once

#include <vulkan/vulkan.h>
#include "Graphics/Descriptors/DescriptorSet.hpp"

namespace acid
{
/**
 * @brief Interface that represents a buffer.
 */
class Buffer
{
public:
	enum class Status
	{
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

	void MapMemory(void **data);

	void UnmapMemory();

	const VkDeviceSize &GetSize() const { return m_size; }

	const VkBuffer &GetBuffer() const { return m_buffer; }

	const VkDeviceMemory &GetBufferMemory() const { return m_bufferMemory; }

	static uint32_t FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &requiredProperties);

protected:
	VkDeviceSize m_size;
	VkBuffer m_buffer;
	VkDeviceMemory m_bufferMemory;
};
}
