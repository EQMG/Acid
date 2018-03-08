#include "DescriptorSet.hpp"

#include "../../Devices/Display.hpp"
#include "../Pipelines/Pipeline.hpp"

namespace Flounder
{
	DescriptorSet::DescriptorSet(const Pipeline &pipeline) :
		m_descriptorSet(VK_NULL_HANDLE),
		m_writeSet(std::vector<VkWriteDescriptorSet>())
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkDescriptorSetLayout layouts[1] = {pipeline.GetDescriptorSetLayout()};

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = pipeline.GetDescriptorPool();
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = layouts;

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &m_descriptorSet));
	}

	DescriptorSet::~DescriptorSet()
	{
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		m_writeSet.clear();
		std::copy(descriptorWrites.begin(), descriptorWrites.end(), std::back_inserter(m_writeSet));
	}

	void DescriptorSet::BindDescriptor(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		VkDescriptorSet descriptors[1] = {m_descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
	}
}
