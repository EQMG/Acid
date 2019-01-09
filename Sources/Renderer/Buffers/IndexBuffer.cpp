#include "IndexBuffer.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
	IndexBuffer::IndexBuffer(const VkIndexType &indexType, const uint64_t &elementSize, const std::size_t &indexCount, const void *newData) :
		Buffer(elementSize * indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
		m_indexType(indexType),
		m_indexCount(static_cast<uint32_t>(indexCount))
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		// Copies the index data to the buffer.
		void *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, static_cast<std::size_t>(m_size));
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory);
	}
}
