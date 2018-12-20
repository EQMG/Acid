#include "Buffer.hpp"

#include <cassert>
#include "Display/Display.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
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

		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto graphicsFamily = Display::Get()->GetGraphicsFamily();
		auto presentFamily = Display::Get()->GetPresentFamily();
		auto computeFamily = Display::Get()->GetComputeFamily();

		std::array<uint32_t, 3> queueFamily = {graphicsFamily, presentFamily, computeFamily};

		VkBufferCreateInfo bufferCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = size,
			.usage = usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size()),
			.pQueueFamilyIndices = queueFamily.data()
		};
		Display::CheckVk(vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, &m_buffer));

		// Allocates buffer memory.
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, m_buffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = memoryRequirements.size,
			.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties)
		};
		Display::CheckVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &m_bufferMemory));

		vkBindBufferMemory(logicalDevice, m_buffer, m_bufferMemory, 0);
	}

	Buffer::~Buffer()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyBuffer(logicalDevice, m_buffer, nullptr);
		vkFreeMemory(logicalDevice, m_bufferMemory, nullptr);
	}

	uint32_t Buffer::FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &requiredProperties)
	{
		auto physicalDevice = Display::Get()->GetPhysicalDevice();

		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			uint32_t memoryTypeBits = 1 << i;
			bool isRequiredMemoryType = typeFilter & memoryTypeBits;

			auto properties = physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags;
			bool hasRequiredProperties = (properties & requiredProperties) == requiredProperties;

			if (isRequiredMemoryType && hasRequiredProperties)
			{
				return i;
			}
		}

		assert(false && "Failed to find a valid memory type for buffer!");
		return 0;
	}

	void Buffer::CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size)
	{
		CommandBuffer commandBuffer = CommandBuffer();

		VkBufferCopy copyRegion = {
			.size = size
		};
		vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), srcBuffer, dstBuffer, 1, &copyRegion);

		commandBuffer.End();
		commandBuffer.Submit();
	}
}
