#include "Buffer.hpp"
#include "../queue/QueueFamily.hpp"

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

	void Buffer::Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;

		QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(physicalDevice, surface);
		uint32_t indicesArray[] = { static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.transferFamily) };
		bufferInfo.pQueueFamilyIndices = indicesArray;
		bufferInfo.queueFamilyIndexCount = 2;

		GlfwVulkan::ErrorVk(vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &m_buffer));

		// Allocates buffer memory.
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, m_buffer, &memRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memRequirements.size;
		allocateInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

		GlfwVulkan::ErrorVk(vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &m_bufferMemory));

		vkBindBufferMemory(logicalDevice, m_buffer, m_bufferMemory, 0);
	}

	void Buffer::Cleanup(const VkDevice &logicalDevice)
	{
		vkDestroyBuffer(logicalDevice, m_buffer, nullptr);
		vkFreeMemory(logicalDevice, m_bufferMemory, nullptr);

		m_buffer = VK_NULL_HANDLE;
		m_bufferMemory = VK_NULL_HANDLE;
	}

	uint32_t Buffer::FindMemoryType(const VkPhysicalDevice &physicalDevice, const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			// If typefilter has a bit set to 1 and it contains the properties we indicated.
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find a valid memory type for buffer!");
	}
}
