#include "VertexBuffer.hpp"

#include "../../devices/Display.hpp"

namespace Flounder
{
	VertexBuffer::VertexBuffer() :
		Buffer(),
		m_vertexCount(0)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	void VertexBuffer::Create(const uint64_t &elementSize, const size_t &vertexCount, void *newData)
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		// Calculates the buffers size.
		m_vertexCount = static_cast<uint32_t>(vertexCount);
		const VkDeviceSize bufferSize = elementSize * m_vertexCount;

		// Creates the buffer.
		Buffer::Create(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		// Copies the vertex data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, newData, static_cast<size_t>(bufferSize));
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}

	void VertexBuffer::Cleanup()
	{
		Buffer::Cleanup();
	}
}
