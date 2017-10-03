#include "IndexBuffer.hpp"

#include "../../devices/Display.hpp"

namespace Flounder
{
	IndexBuffer::IndexBuffer() :
		Buffer(),
		m_indexType(VK_INDEX_TYPE_UINT16),
		m_indexCount(0)
	{
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	void IndexBuffer::Create(const VkIndexType &indexType, const uint64_t &elementSize, const size_t &indexCount, void *newData)
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		// Calculates the buffers size.
		m_indexType = indexType;
		m_indexCount = static_cast<uint32_t>(indexCount);
		const VkDeviceSize bufferSize = elementSize * m_indexCount;

		// Creates the buffer.
		Buffer::Create(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		// Copies the index data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, newData, static_cast<size_t>(bufferSize));
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}

	void IndexBuffer::Cleanup()
	{
		Buffer::Cleanup();
	}
}
