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

	void Buffer::Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
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

		if (vkCreateBuffer(*logicalDevice, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create buffer");
		}
		else
		{
			printf("Buffer created successfully!\n");
		}

		// Allocates buffer memory.
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(*logicalDevice, m_buffer, &memRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memRequirements.size;
		allocateInfo.memoryTypeIndex = FindMemoryType(*physicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(*logicalDevice, &allocateInfo, nullptr, &m_bufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate memory for buffer");
		}
		else
		{
			printf("Memory allocated for buffer successfully!\n");
		}

		vkBindBufferMemory(*logicalDevice, m_buffer, m_bufferMemory, 0);
	}

	void Buffer::Cleanup(const VkDevice *logicalDevice)
	{
		vkDestroyBuffer(*logicalDevice, m_buffer, nullptr);
		vkFreeMemory(*logicalDevice, m_bufferMemory, nullptr);
		m_buffer = VK_NULL_HANDLE;
		m_bufferMemory = VK_NULL_HANDLE;
	}

	uint32_t Buffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			// If typefilter has a bit set to 1 and it contains the properties we indicated.
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				printf("Valid memory type found for buffer!\n");
				return i;
			}
		}

		throw std::runtime_error("Failed to find a valid memory type for buffer");
	}
}
