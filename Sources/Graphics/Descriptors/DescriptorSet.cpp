#include "DescriptorSet.hpp"

#include "Graphics/Graphics.hpp"

namespace acid
{
DescriptorSet::DescriptorSet(const Pipeline &pipeline) :
	m_pipelineLayout{pipeline.GetPipelineLayout()},
	m_pipelineBindPoint{pipeline.GetPipelineBindPoint()},
	m_descriptorPool{pipeline.GetDescriptorPool()}
{
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};

	VkDescriptorSetLayout layouts[1]{ pipeline.GetDescriptorSetLayout() };

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = layouts;
	Graphics::CheckVk(vkAllocateDescriptorSets(*logicalDevice, &descriptorSetAllocateInfo, &m_descriptorSet));
}

DescriptorSet::~DescriptorSet()
{
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};

	Graphics::CheckVk(vkFreeDescriptorSets(*logicalDevice, m_descriptorPool, 1, &m_descriptorSet));
}

void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites)
{
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};

	vkUpdateDescriptorSets(*logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void DescriptorSet::BindDescriptor(const CommandBuffer &commandBuffer) const
{
	vkCmdBindDescriptorSets(commandBuffer, m_pipelineBindPoint, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);
}
}
