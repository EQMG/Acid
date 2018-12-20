#include "StorageBuffer.hpp"

#include "Display/Display.hpp"

namespace acid
{
	StorageBuffer::StorageBuffer(const VkDeviceSize &size) :
		IDescriptor(),
		Buffer(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT), // TODO: Use DEVICE_LOCAL instead.
		m_bufferInfo({})
	{
		m_bufferInfo = {
			.buffer = m_buffer,
			.offset = 0,
			.range = m_size
		};
	}

	void StorageBuffer::Update(const void *newData)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Copies the data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, static_cast<size_t>(m_size));
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}

	DescriptorType StorageBuffer::CreateDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
			.binding = binding,
			.descriptorType = descriptorType,
			.descriptorCount = 1,
			.stageFlags = stage,
			.pImmutableSamplers = nullptr
		};
		VkDescriptorPoolSize descriptorPoolSize = {
			.type = descriptorType,
			.descriptorCount = count
		};
		return DescriptorType(binding, stage, descriptorSetLayoutBinding, descriptorPoolSize);
	}

	VkWriteDescriptorSet StorageBuffer::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const DescriptorSet &descriptorSet) const
	{
		VkWriteDescriptorSet descriptorWrite = {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = descriptorSet.GetDescriptorSet(),
			.dstBinding = binding,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = descriptorType,
			.pBufferInfo = &m_bufferInfo
		};
		return descriptorWrite;
	}
}
