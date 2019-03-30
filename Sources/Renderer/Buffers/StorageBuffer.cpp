#include "StorageBuffer.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
StorageBuffer::StorageBuffer(const VkDeviceSize &size, const void *data) :
	Buffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, data)
{
}

void StorageBuffer::Update(const void *newData)
{
	void *data;
	Buffer::MapMemory(&data);
	std::memcpy(data, newData, static_cast<std::size_t>(m_size));
	Buffer::UnmapMemory();
}

VkDescriptorSetLayoutBinding StorageBuffer::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage,
	const uint32_t &count)
{
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
	descriptorSetLayoutBinding.binding = binding;
	descriptorSetLayoutBinding.descriptorType = descriptorType;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = stage;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
	return descriptorSetLayoutBinding;
}

WriteDescriptorSet StorageBuffer::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const
{
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = m_buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = m_size;

	if (offsetSize)
	{
		bufferInfo.offset = offsetSize->GetOffset();
		bufferInfo.range = offsetSize->GetSize();
	}

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = VK_NULL_HANDLE; // Will be set in the descriptor handler.
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.descriptorType = descriptorType;
	//descriptorWrite.pBufferInfo = &bufferInfo;
	return WriteDescriptorSet(descriptorWrite, bufferInfo);
}
}
