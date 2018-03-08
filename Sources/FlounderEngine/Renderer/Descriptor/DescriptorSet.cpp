#include "DescriptorSet.hpp"

#include "../../Devices/Display.hpp"
#include "../Pipelines/Pipeline.hpp"

namespace Flounder
{
	DescriptorSet::DescriptorSet(const PipelineCreate &pipelineCreateInfo) :
		m_pipelineCreateInfo(pipelineCreateInfo),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_descriptorSet(VK_NULL_HANDLE)
	{
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreateDescriptorSet();
	}

	DescriptorSet::~DescriptorSet()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::BindDescriptor(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		VkDescriptorSet descriptors[1] = {m_descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
	}

	void DescriptorSet::CreateDescriptorLayout()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorSetLayoutBinding> bindings = std::vector<VkDescriptorSetLayoutBinding>();

		for (auto type : m_pipelineCreateInfo.descriptors)
		{
			bindings.push_back(type.m_descriptorSetLayoutBinding);
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void DescriptorSet::CreateDescriptorPool()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorPoolSize> poolSizes = std::vector<VkDescriptorPoolSize>();

		for (auto type : m_pipelineCreateInfo.descriptors)
		{
			poolSizes.push_back(type.m_descriptorPoolSize);
		}

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
		descriptorPoolCreateInfo.maxSets = 50;

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
	}

	void DescriptorSet::CreateDescriptorSet()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = &m_descriptorSetLayout;

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &m_descriptorSet));
	}
}
