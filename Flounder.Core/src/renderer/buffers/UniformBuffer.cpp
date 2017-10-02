#include "UniformBuffer.hpp"

#include <vector>

namespace Flounder
{
	UniformBuffer::UniformBuffer() :
		m_size(0),
		m_binding(0),
		m_descriptorSetLayout(VK_NULL_HANDLE)
	{
	}

	UniformBuffer::UniformBuffer(const VkDeviceSize &size, const uint32_t &binding) :
		m_size(size),
		m_binding(binding),
		m_descriptorSetLayout(VK_NULL_HANDLE)
	{
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	void UniformBuffer::Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface)
	{
		Buffer::Create(logicalDevice, physicalDevice, surface, m_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = 0; // m_binding;
		descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = 1;
		descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;

		GlfwVulkan::ErrorVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void UniformBuffer::Cleanup(const VkDevice &logicalDevice)
	{
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
