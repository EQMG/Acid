#include "DescriptorSet.hpp"

#include "Devices/Window.hpp"
#include "Descriptor.hpp"

namespace acid
{
	DescriptorSet::DescriptorSet(const Pipeline &pipeline) :
		m_pipelineLayout(pipeline.GetPipelineLayout()),
		m_pipelineBindPoint(pipeline.GetPipelineBindPoint()),
		m_descriptorPool(pipeline.GetDescriptorPool()),
		m_descriptorSet(VK_NULL_HANDLE)
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();

		VkDescriptorSetLayout layouts[1] = { pipeline.GetDescriptorSetLayout() };

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = layouts;
		Window::CheckVk(vkAllocateDescriptorSets(logicalDevice->GetLogicalDevice(), &descriptorSetAllocateInfo, &m_descriptorSet));
	}

	DescriptorSet::~DescriptorSet()
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();

		Window::CheckVk(vkFreeDescriptorSets(logicalDevice->GetLogicalDevice(), m_descriptorPool, 1, &m_descriptorSet));
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites)
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();

		vkUpdateDescriptorSets(logicalDevice->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::BindDescriptor(const CommandBuffer &commandBuffer)
	{
		vkCmdBindDescriptorSets(commandBuffer.GetCommandBuffer(), m_pipelineBindPoint, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);
	}
}
