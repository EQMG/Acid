#include "DescriptorSet.hpp"

#include "IDescriptor.hpp"

namespace acid
{
	DescriptorSet::DescriptorSet(const IPipeline &pipeline) :
		m_shaderProgram(pipeline.GetShaderProgram()),
		m_pipelineLayout(pipeline.GetVkPipelineLayout()),
		m_pipelineBindPoint(pipeline.GetVkPipelineBindPoint()),
		m_descriptorPool(pipeline.GetVkDescriptorPool()),
		m_descriptorSet(VK_NULL_HANDLE)
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkDescriptorSetLayout layouts[1] = {pipeline.GetVkDescriptorSetLayout()};

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = layouts;

		Display::CheckVk(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &m_descriptorSet));
	}

	DescriptorSet::~DescriptorSet()
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkDescriptorSet descriptors[1] = {m_descriptorSet};
		vkFreeDescriptorSets(logicalDevice, m_descriptorPool, 1, descriptors);
	}

	void DescriptorSet::Update(const std::vector<IDescriptor *> &descriptors)
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		std::vector<VkWriteDescriptorSet> descriptorWrites = {};

		for (uint32_t i = 0; i < descriptors.size(); i++)
		{
			if (descriptors.at(i) != nullptr)
			{
				descriptorWrites.emplace_back(descriptors.at(i)->GetVkWriteDescriptor(i, *this));
			}
		}

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::BindDescriptor(const CommandBuffer &commandBuffer)
	{
		VkDescriptorSet descriptors[1] = {m_descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(), m_pipelineBindPoint, m_pipelineLayout, 0, 1, descriptors, 0, nullptr);
	}
}
