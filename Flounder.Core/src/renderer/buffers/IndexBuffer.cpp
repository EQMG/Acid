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

	void IndexBuffer::Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, VkCommandPool *transferCommandPool, const VkQueue *transferQueue)
	{
		VkDeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();
		Buffer bufferStaging = Buffer();
		bufferStaging.Create(logicalDevice, physicalDevice, surface, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		CopyVerticesToBuffer(logicalDevice, bufferSize, bufferStaging);

		Buffer::Create(logicalDevice, physicalDevice, surface, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CopyBuffer(logicalDevice, transferCommandPool, *bufferStaging.GetBuffer(), *GetBuffer(), bufferSize, transferQueue);

		bufferStaging.Cleanup(logicalDevice);
	}

	void IndexBuffer::Cleanup(const VkDevice *logicalDevice)
	{
		Buffer::Cleanup(logicalDevice);
	}

	void IndexBuffer::SetIndices(std::vector<uint16_t> indices)
	{
		m_indices.clear();
		m_indices.swap(indices);
	}

	void IndexBuffer::CopyVerticesToBuffer(const VkDevice *logicalDevice, const VkDeviceSize &bufferSize, Buffer &bufferStaging) const
	{
		// Copies the vertex data to the buffer.
		void *data;
		vkMapMemory(*logicalDevice, *bufferStaging.GetBufferMemory(), 0, bufferSize, 0, &data);
		memcpy(data, m_indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(*logicalDevice, *bufferStaging.GetBufferMemory());
	}

	void IndexBuffer::CopyBuffer(const VkDevice *logicalDevice, const VkCommandPool *transferCommandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkQueue *transferQueue)
	{
		// Makes a temporary command buffer for the memory transfer operation.
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = *transferCommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(*logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(*transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(*transferQueue);

		vkFreeCommandBuffers(*logicalDevice, *transferCommandPool, 1, &commandBuffer);
	}
}
