#include "UniformBuffer.hpp"

#include "../../devices/Display.hpp"

namespace Flounder
{
	UniformBuffer::UniformBuffer(const VkDeviceSize &size, const VkShaderStageFlags &stage) :
		Buffer(),
		m_size(size),
		m_stage(stage)
	{
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	void UniformBuffer::Create()
	{
		// Creates the buffer.
		Buffer::Create(m_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}

	void UniformBuffer::Cleanup()
	{
		Buffer::Cleanup();
	}

	void UniformBuffer::Update(void *newData)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Copies the data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, m_size);
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}

	VkDescriptorSetLayoutBinding UniformBuffer::GetDescriptorLayout(const uint32_t &binding)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = m_stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

		return descriptorSetLayoutBinding;
	}

	VkDescriptorPoolSize UniformBuffer::GetDescriptorPool(const uint32_t &binding)
	{
		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorPoolSize.descriptorCount = 1;

		return descriptorPoolSize;
	}

	VkWriteDescriptorSet UniformBuffer::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet)
	{
		// TODO: Don't create a descriptor like this!
		VkDescriptorBufferInfo *descriptorInfo = new VkDescriptorBufferInfo();
		descriptorInfo->buffer = m_buffer;
		descriptorInfo->offset = 0;
		descriptorInfo->range = m_size;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = descriptorInfo;

		return descriptorWrite;
	}
}
