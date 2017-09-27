#include "IndexBuffer.hpp"

namespace Flounder
{
	IndexBuffer::IndexBuffer() :
		m_indices(std::vector<uint16_t>())
	{
	}

	IndexBuffer::IndexBuffer(const std::vector<uint16_t> &indices) :
		m_indices(std::vector<uint16_t>(indices))
	{
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	void IndexBuffer::Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkQueue &queue, const VkCommandPool &transferCommandPool)
	{
		const VkDeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

		Buffer bufferStaging = Buffer();
		bufferStaging.Create(logicalDevice, physicalDevice, surface, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		CopyVerticesToBuffer(logicalDevice, bufferSize, bufferStaging);

		Buffer::Create(logicalDevice, physicalDevice, surface, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		Buffer::CopyBuffer(logicalDevice, queue, transferCommandPool, bufferStaging.GetBuffer(), GetBuffer(), bufferSize);

		bufferStaging.Cleanup(logicalDevice);
	}

	void IndexBuffer::Cleanup(const VkDevice &logicalDevice)
	{
		Buffer::Cleanup(logicalDevice);
	}

	void IndexBuffer::SetIndices(std::vector<uint16_t> indices)
	{
		m_indices.clear();
		m_indices.swap(indices);
	}

	void IndexBuffer::CopyVerticesToBuffer(const VkDevice &logicalDevice, const VkDeviceSize &bufferSize, Buffer &bufferStaging) const
	{
		// Copies the vertex data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, bufferStaging.GetBufferMemory(), 0, bufferSize, 0, &data);
		memcpy(data, m_indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(logicalDevice, bufferStaging.GetBufferMemory());
	}
}
