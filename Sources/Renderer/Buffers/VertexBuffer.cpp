#include "VertexBuffer.hpp"

namespace acid
{
	VertexBuffer::VertexBuffer(const uint64_t &elementSize, const size_t &vertexCount, void *newData) :
		Buffer(elementSize * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
		m_vertexCount(static_cast<uint32_t>(vertexCount))
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		// Copies the vertex data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, static_cast<size_t>(m_size));
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}

	VertexBuffer::~VertexBuffer()
	{
	}
}
