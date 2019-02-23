#include "UniformBuffer.hpp"

#include <cstring>
#include "Renderer/Renderer.hpp"

namespace acid
{
	UniformBuffer::UniformBuffer(const VkDeviceSize &size, const void *data) :
		Buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, data)
	{
	}

	void UniformBuffer::Update(const void *newData)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		// Copies the data to the buffer.
		void *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, static_cast<std::size_t>(m_size));
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), m_bufferMemory);
	}

	VkDescriptorSetLayoutBinding UniformBuffer::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, 
		const VkShaderStageFlags &stage, const uint32_t &count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}

	WriteDescriptorSet UniformBuffer::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType,
		const VkDescriptorSet &descriptorSet, const std::optional<OffsetSize> &offsetSize) const
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
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = descriptorType;
	//	descriptorWrite.pBufferInfo = &bufferInfo;
		return WriteDescriptorSet(descriptorWrite, bufferInfo);
	}
}
