#include "Buffer.hpp"

#include "Renderer/Queue/QueueFamily.hpp"
#include "Renderer/Renderer.hpp"

namespace fl
{
	Buffer::Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties) :
		m_size(size),
		m_buffer(VK_NULL_HANDLE),
		m_bufferMemory(VK_NULL_HANDLE)
	{
		if (m_size == 0)
		{
			return;
		}

		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto surface = Display::Get()->GetVkSurface();
		auto indices = QueueFamily::FindQueueFamilies(surface);

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(indices.GetArray().size());
		bufferCreateInfo.pQueueFamilyIndices = indices.GetArray().data();

		Display::ErrorVk(vkCreateBuffer(logicalDevice, &bufferCreateInfo, allocator, &m_buffer));

		// Allocates buffer memory.
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, m_buffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		Display::ErrorVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, allocator, &m_bufferMemory));

		vkBindBufferMemory(logicalDevice, m_buffer, m_bufferMemory, 0);
	}

	Buffer::~Buffer()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		vkDestroyBuffer(logicalDevice, m_buffer, allocator);
		vkFreeMemory(logicalDevice, m_bufferMemory, allocator);
	}

	uint32_t Buffer::FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties)
	{
		auto physicalDevice = Display::Get()->GetVkPhysicalDevice();

		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			// If typefilter has a bit set to 1 and it contains the properties we indicated.
			if ((typeFilter & (1 << i)) &&
				(physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find a valid memory type for buffer!");
		return 0;
	}

	void Buffer::CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size)
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto queue = Display::Get()->GetVkQueue();
		auto commandPool = Renderer::Get()->GetVkCommandPool();

		// Makes a temporary command buffer for the memory transfer operation.
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &commandBuffer);

		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(queue);

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	}
}
