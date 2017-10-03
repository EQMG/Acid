#include "Buffer.hpp"

#include <cassert>
#include "../../devices/Display.hpp"
#include "../Renderer.hpp"

namespace Flounder
{
	Buffer::Buffer() :
		m_buffer(VK_NULL_HANDLE),
		m_bufferMemory(VK_NULL_HANDLE)
	{
	}

	Buffer::~Buffer()
	{
	}

	void Buffer::Create(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties)
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		//	QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(physicalDevice, surface);
		//	uint32_t indicesArray[] = { static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.transferFamily) };

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // VK_SHARING_MODE_CONCURRENT
		//	bufferInfo.queueFamilyIndexCount = 2;
		//	bufferInfo.pQueueFamilyIndices = indicesArray;

		GlfwVulkan::ErrorVk(vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, &m_buffer));

		// Allocates buffer memory.
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, m_buffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);
		
		GlfwVulkan::ErrorVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &m_bufferMemory));

		vkBindBufferMemory(logicalDevice, m_buffer, m_bufferMemory, 0);
	}

	void Buffer::Cleanup()
	{
		const auto logicalDevice = Display::Get()->GetDevice();
		vkDestroyBuffer(logicalDevice, m_buffer, nullptr);
		vkFreeMemory(logicalDevice, m_bufferMemory, nullptr);
	}

	uint32_t Buffer::FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties)
	{
		const auto physicalDevice = Display::Get()->GetPhysicalDevice();

		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			// If typefilter has a bit set to 1 and it contains the properties we indicated.
			if ((typeFilter & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		assert(false && "Failed to find a valid memory type for buffer!");
		return 0;
	}

	void Buffer::CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size)
	{
		const auto logicalDevice = Display::Get()->GetDevice();
		const auto queue = Display::Get()->GetQueue();
		const auto commandPool = Renderer::Get()->GetCommandPool();

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
