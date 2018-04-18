#include "UniformBuffer.hpp"

namespace Flounder
{
	UniformBuffer::UniformBuffer(const VkDeviceSize &size) :
		Buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
		IDescriptor(),
		m_bufferInfo({})
	{
		m_bufferInfo.buffer = m_buffer;
		m_bufferInfo.offset = 0;
		m_bufferInfo.range = m_size;
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	void UniformBuffer::Update(void *newData)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		//	const auto commandBuffer = Renderer::Get()->GetCommandBuffer();
		//	vkCmdUpdateBuffer(commandBuffer, m_buffer, 0, m_size, newData);

		// Copies the data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, static_cast<size_t>(m_size));
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}

	void UniformBuffer::UpdateMap(const std::string &blockName, ShaderProgram *shaderProgram, const std::map<std::string, void *> &newDataMap)
	{
		/*void *newData = malloc(m_size);
		void *offset = newData;

		for (auto pair : newDataMap)
		{
			if (shaderProgram->IsDescriptorDefined(pair.first))
			{
				Uniform *uniform = shaderProgram->GetBlockUniform(blockName, pair.first);
				memcpy(offset, pair.second, uniform->m_size);
				offset += uniform->m_offset;
			}
		}

		Update(newData);
		free(newData);*/
	}

	DescriptorType UniformBuffer::CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		descriptorSetLayoutBinding.stageFlags = stage;

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorPoolSize.descriptorCount = 1;

		return DescriptorType(binding, stage, descriptorSetLayoutBinding, descriptorPoolSize);
	}

	VkWriteDescriptorSet UniformBuffer::GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet.GetDescriptorSet();
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &m_bufferInfo;

		return descriptorWrite;
	}
}
