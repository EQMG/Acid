#include "Buffer.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
Buffer::Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, const void *data) :
	m_size(size) {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto graphicsFamily = logicalDevice->GetGraphicsFamily();
	auto presentFamily = logicalDevice->GetPresentFamily();
	auto computeFamily = logicalDevice->GetComputeFamily();

	std::array queueFamily = {graphicsFamily, presentFamily, computeFamily};

	// Create the buffer handle.
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
	bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();
	Graphics::CheckVk(vkCreateBuffer(*logicalDevice, &bufferCreateInfo, nullptr, &m_buffer));

	// Create the memory backing up the buffer handle.
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(*logicalDevice, m_buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);
	Graphics::CheckVk(vkAllocateMemory(*logicalDevice, &memoryAllocateInfo, nullptr, &m_bufferMemory));

	// If a pointer to the buffer data has been passed, map the buffer and copy over the data.
	if (data) {
		void *mapped;
		MapMemory(&mapped);
		std::memcpy(mapped, data, size);

		// If host coherency hasn't been requested, do a manual flush to make writes visible.
		if ((properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0) {
			VkMappedMemoryRange mappedMemoryRange = {};
			mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			mappedMemoryRange.memory = m_bufferMemory;
			mappedMemoryRange.offset = 0;
			mappedMemoryRange.size = size;
			vkFlushMappedMemoryRanges(*logicalDevice, 1, &mappedMemoryRange);
		}

		UnmapMemory();
	}

	// Attach the memory to the buffer object.
	Graphics::CheckVk(vkBindBufferMemory(*logicalDevice, m_buffer, m_bufferMemory, 0));
}

Buffer::~Buffer() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	vkDestroyBuffer(*logicalDevice, m_buffer, nullptr);
	vkFreeMemory(*logicalDevice, m_bufferMemory, nullptr);
}

void Buffer::MapMemory(void **data) const {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();
	Graphics::CheckVk(vkMapMemory(*logicalDevice, GetBufferMemory(), 0, m_size, 0, data));
}

void Buffer::UnmapMemory() const {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();
	vkUnmapMemory(*logicalDevice, GetBufferMemory());
}

uint32_t Buffer::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags &requiredProperties) {
	auto physicalDevice = Graphics::Get()->GetPhysicalDevice();
	auto memoryProperties = physicalDevice->GetMemoryProperties();

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		uint32_t memoryTypeBits = 1 << i;

		if (typeFilter & memoryTypeBits && (memoryProperties.memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find a valid memory type for buffer");
}

void Buffer::InsertBufferMemoryBarrier(const CommandBuffer &commandBuffer, const VkBuffer &buffer, const VkAccessFlags &srcAccessMask, const VkAccessFlags &dstAccessMask,
	const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask, const VkDeviceSize &offset, const VkDeviceSize &size) {
	VkBufferMemoryBarrier bufferMemoryBarrier = {};
	bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	bufferMemoryBarrier.srcAccessMask = srcAccessMask;
	bufferMemoryBarrier.dstAccessMask = dstAccessMask;
	bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemoryBarrier.buffer = buffer;
	bufferMemoryBarrier.offset = offset;
	bufferMemoryBarrier.size = size;
	vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 1, &bufferMemoryBarrier, 0, nullptr);
}
}
