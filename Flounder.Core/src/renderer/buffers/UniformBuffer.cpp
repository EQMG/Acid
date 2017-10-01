#include "UniformBuffer.hpp"

namespace Flounder
{
	UniformBuffer::UniformBuffer() :
		m_size(0),
		m_binding(0),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_descriptorSet(VK_NULL_HANDLE)
	{
	}

	UniformBuffer::UniformBuffer(const VkDeviceSize &size, const uint32_t &binding) :
		m_size(size),
		m_binding(binding),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_descriptorSet(VK_NULL_HANDLE)
	{
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	void UniformBuffer::Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface)
	{
		Buffer::Create(logicalDevice, physicalDevice, surface, m_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		{
			VkDescriptorSetLayoutBinding uboLayoutBinding = {};
			uboLayoutBinding.binding = m_binding;
			uboLayoutBinding.descriptorCount = 1;
			uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uboLayoutBinding.pImmutableSamplers = nullptr;
			uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			layoutInfo.bindingCount = 1;
			layoutInfo.pBindings = &uboLayoutBinding;

			GlfwVulkan::ErrorVk(vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout));
		}
		{
			VkDescriptorPoolSize poolSize = {};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize.descriptorCount = 1;

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = 1;

			GlfwVulkan::ErrorVk(vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &m_descriptorPool));
		}
		{
			VkDescriptorSetLayout layouts[] = { m_descriptorSetLayout };
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.pNext = nullptr;
			allocInfo.descriptorPool = m_descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = layouts;

			GlfwVulkan::ErrorVk(vkAllocateDescriptorSets(logicalDevice, &allocInfo, &m_descriptorSet));
		}
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = Buffer::GetBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = m_size;

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSet;
			descriptorWrite.dstBinding = m_binding;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);
		}
	}

	void UniformBuffer::Cleanup(const VkDevice &logicalDevice)
	{
		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, nullptr);
		Buffer::Cleanup(logicalDevice);
	}

	void UniformBuffer::Update(const VkDevice &logicalDevice, void *newData)
	{
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, m_size);
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}
}
