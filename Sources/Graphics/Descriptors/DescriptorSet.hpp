#pragma once

#include "Graphics/Commands/CommandBuffer.hpp"
#include "Graphics/Pipelines/Pipeline.hpp"

namespace acid
{
class Descriptor;
class WriteDescriptorSet;

class DescriptorSet
{
public:
	explicit DescriptorSet(const Pipeline &pipeline);

	~DescriptorSet();

	void Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites);

	void BindDescriptor(const CommandBuffer &commandBuffer);

	const VkDescriptorSet &GetDescriptorSet() const { return m_descriptorSet; }

private:
	VkPipelineLayout m_pipelineLayout;
	VkPipelineBindPoint m_pipelineBindPoint;
	VkDescriptorPool m_descriptorPool;
	VkDescriptorSet m_descriptorSet;
};
}
