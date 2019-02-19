#include "Buffer.hpp"

#include <array>
#include <cassert>
#include "Renderer/Renderer.hpp"

namespace acid
{
	Buffer::Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, const void *data) :
		m_size(size),
		m_buffer(VK_NULL_HANDLE),
		m_bufferMemory(VK_NULL_HANDLE)
	{
		if (m_size == 0)
		{
			return;
		}

		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		auto graphicsFamily = logicalDevice->GetGraphicsFamily();
		auto presentFamily = logicalDevice->GetPresentFamily();
		auto computeFamily = logicalDevice->GetComputeFamily();

		std::array<uint32_t, 3> queueFamily = {graphicsFamily, presentFamily, computeFamily};

		// Create the buffer handle.
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//	bufferCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
		//	bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();
		Renderer::CheckVk(vkCreateBuffer(logicalDevice->GetLogicalDevice(), &bufferCreateInfo, nullptr, &m_buffer));

		// Create the memory backing up the buffer handle.
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice->GetLogicalDevice(), m_buffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);
		Renderer::CheckVk(vkAllocateMemory(logicalDevice->GetLogicalDevice(), &memoryAllocateInfo, nullptr, &m_bufferMemory));

		// If a pointer to the buffer data has been passed, map the buffer and copy over the data.
		if (data != nullptr)
		{
			void *mapped;
			Renderer::CheckVk(vkMapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory, 0, size, 0, &mapped));
			memcpy(mapped, data, size);

			// If host coherency hasn't been requested, do a manual flush to make writes visible.
			if ((properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
			{
				VkMappedMemoryRange mappedMemoryRange = {};
				mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				mappedMemoryRange.memory = m_bufferMemory;
				mappedMemoryRange.offset = 0;
				mappedMemoryRange.size = size;
				vkFlushMappedMemoryRanges(logicalDevice->GetLogicalDevice(), 1, &mappedMemoryRange);
			}

			vkUnmapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory);
		}

		// Attach the memory to the buffer object.
		Renderer::CheckVk(vkBindBufferMemory(logicalDevice->GetLogicalDevice(), m_buffer, m_bufferMemory, 0));
	}

	Buffer::~Buffer()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		vkDestroyBuffer(logicalDevice->GetLogicalDevice(), m_buffer, nullptr);
		vkFreeMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory, nullptr);
	}

	void Buffer::CopyBuffer(void *data) const
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		Renderer::CheckVk(vkMapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory, 0, m_size, 0, &data));
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory);
	}

	uint32_t Buffer::FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &requiredProperties)
	{
		auto physicalDevice = Renderer::Get()->GetPhysicalDevice();

		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice->GetPhysicalDevice(), &physicalDeviceMemoryProperties);

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

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), srcBuffer, dstBuffer, 1, &copyRegion);

		commandBuffer.End();
		commandBuffer.SubmitIdle();
	}
}
