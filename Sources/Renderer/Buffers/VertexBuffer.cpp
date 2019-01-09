#include "VertexBuffer.hpp"

#include "Devices/Window.hpp"

namespace acid
{
	VertexBuffer::VertexBuffer(const uint64_t &elementSize, const std::size_t &vertexCount, const void *newData) :
		Buffer(elementSize * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
		m_vertexCount(static_cast<uint32_t>(vertexCount))
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();

		// Copies the vertex data to the buffer.
		void *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, static_cast<std::size_t>(m_size));
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory);
	}
}
