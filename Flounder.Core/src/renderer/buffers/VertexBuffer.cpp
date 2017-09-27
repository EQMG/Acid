#include "VertexBuffer.hpp"

namespace Flounder
{
	VertexBuffer::VertexBuffer() :
		m_vertices(std::vector<Vertex>())
	{
	}

	VertexBuffer::VertexBuffer(const std::vector<Vertex> &vertices) :
		m_vertices(std::vector<Vertex>(vertices))
	{
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	void VertexBuffer::Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkQueue &queue, const VkCommandPool &transferCommandPool)
	{
		const VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

		Buffer bufferStaging = Buffer();
		bufferStaging.Create(logicalDevice, physicalDevice, surface, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		CopyVerticesToBuffer(logicalDevice, bufferSize, bufferStaging);

		Buffer::Create(logicalDevice, physicalDevice, surface, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		Buffer::CopyBuffer(logicalDevice, queue, transferCommandPool, bufferStaging.GetBuffer(), GetBuffer(), bufferSize);

		bufferStaging.Cleanup(logicalDevice);
	}

	void VertexBuffer::Cleanup(const VkDevice &logicalDevice)
	{
		Buffer::Cleanup(logicalDevice);
	}

	void VertexBuffer::SetVerticies(std::vector<Vertex> &vertices)
	{
		m_vertices.clear();
		m_vertices.swap(vertices);
	}

	void VertexBuffer::CopyVerticesToBuffer(const VkDevice &logicalDevice, const VkDeviceSize &bufferSize, Buffer &bufferStaging) const
	{
		// Copies the vertex data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, bufferStaging.GetBufferMemory(), 0, bufferSize, 0, &data);
		memcpy(data, m_vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(logicalDevice, bufferStaging.GetBufferMemory());
	}
}
