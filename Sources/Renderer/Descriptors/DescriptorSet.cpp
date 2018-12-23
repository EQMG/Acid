#include "DescriptorSet.hpp"

#include "Display/Display.hpp"
#include "IDescriptor.hpp"

namespace acid
{
	DescriptorSet::DescriptorSet(const IPipeline &pipeline) :
		m_pipelineLayout(pipeline.GetPipelineLayout()),
		m_pipelineBindPoint(pipeline.GetPipelineBindPoint()),
		m_descriptorPool(pipeline.GetDescriptorPool()),
		m_descriptorSet(VK_NULL_HANDLE)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkDescriptorSetLayout layouts[1] = { pipeline.GetDescriptorSetLayout() };

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
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Display::CheckVk(vkFreeDescriptorSets(logicalDevice, m_descriptorPool, 1, &m_descriptorSet));
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::Update(const std::vector<WriteDescriptorSet> &descriptorWrites)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkWriteDescriptorSet> descriptors = {};

		for (auto &descriptorWrite : descriptorWrites)
		{
			auto descriptor = static_cast<VkWriteDescriptorSet>(descriptorWrite);
			descriptor.pImageInfo = &descriptorWrite.imageInfo;
			descriptor.pBufferInfo = &descriptorWrite.bufferInfo;
			descriptors.emplace_back(descriptor);
		}

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptors.size()), descriptors.data(), 0, nullptr);
	}

	void DescriptorSet::BindDescriptor(const CommandBuffer &commandBuffer)
	{
		vkCmdBindDescriptorSets(commandBuffer.GetCommandBuffer(), m_pipelineBindPoint, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);
	}
}
