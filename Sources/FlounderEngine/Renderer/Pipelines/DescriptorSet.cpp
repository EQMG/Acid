#include "DescriptorSet.hpp"

#include "Descriptor.hpp"
#include "Pipeline.hpp"

namespace Flounder
{
	DescriptorSet::DescriptorSet(const Pipeline &pipeline) :
		m_shaderProgram(pipeline.GetShaderProgram()),
		m_pipelineLayout(pipeline.GetPipelineLayout()),
		m_descriptorPool(pipeline.GetDescriptorPool()),
		m_descriptorSet(VK_NULL_HANDLE),
		m_descriptors(std::vector<Descriptor *>())
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkDescriptorSetLayout layouts[1] = {pipeline.GetDescriptorSetLayout()};

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = layouts;

		vkDeviceWaitIdle(logicalDevice);
		Display::ErrorVk(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &m_descriptorSet));
	}

	DescriptorSet::~DescriptorSet()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkDescriptorSet descriptors[1] = {m_descriptorSet};
		vkFreeDescriptorSets(logicalDevice, m_descriptorPool, 1, descriptors);
	}

	void DescriptorSet::Update(const std::vector<Descriptor *> &descriptors)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		if (!m_descriptors.empty() && descriptors == m_descriptors)
		{
			return;
		}

		m_descriptors.clear();
		std::copy(descriptors.begin(), descriptors.end(), std::back_inserter(m_descriptors));

		std::vector<VkWriteDescriptorSet> descriptorWrites = {};

		for (unsigned int i = 0; i < descriptors.size(); i++)
		{
			if (descriptors.at(i) != nullptr)
			{
				descriptorWrites.push_back(descriptors.at(i)->GetWriteDescriptor(i, *this));
			}
		}

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::UpdateMap(const std::unordered_map<std::string, Descriptor *> &descriptorMap)
	{
		std::vector<Descriptor *> descriptors = {};

		for (auto pair : descriptorMap)
		{
			if (m_shaderProgram->IsDescriptorDefined(pair.first))
			{
				descriptors.push_back(pair.second);
			}
			//	else
			//	{
			//		descriptors.push_back(nullptr);
			//	}
		}

		Update(descriptors);
	}

	void DescriptorSet::BindDescriptor(const VkCommandBuffer &commandBuffer)
	{
		VkDescriptorSet descriptors[1] = {m_descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, descriptors, 0, nullptr);
	}
}
