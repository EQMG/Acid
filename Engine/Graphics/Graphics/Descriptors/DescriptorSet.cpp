#include "DescriptorSet.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
DescriptorSet::DescriptorSet(const Pipeline &pipeline) :
	pipelineLayout(pipeline.GetPipelineLayout()),
	pipelineBindPoint(pipeline.GetPipelineBindPoint()),
	descriptorPool(pipeline.GetDescriptorPool()) {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	VkDescriptorSetLayout layouts[1] = {pipeline.GetDescriptorSetLayout()};

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = layouts;
	Graphics::CheckVk(vkAllocateDescriptorSets(*logicalDevice, &descriptorSetAllocateInfo, &descriptorSet));
}

DescriptorSet::~DescriptorSet() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	Graphics::CheckVk(vkFreeDescriptorSets(*logicalDevice, descriptorPool, 1, &descriptorSet));
}

void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites) {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	vkUpdateDescriptorSets(*logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void DescriptorSet::BindDescriptor(const CommandBuffer &commandBuffer) const {
	vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
}
}
