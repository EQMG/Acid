#pragma once

#include "Renderer/Commands/CommandBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
class Descriptor;
class WriteDescriptorSet;

class ACID_EXPORT DescriptorSet
{
  public:
	explicit DescriptorSet(const Pipeline& pipeline);

	~DescriptorSet();

	void Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites);

	void BindDescriptor(const CommandBuffer& commandBuffer);

	const VkDescriptorSet& GetDescriptorSet() const
	{
		return m_descriptorSet;
	}

  private:
	VkPipelineLayout m_pipelineLayout;
	VkPipelineBindPoint m_pipelineBindPoint;
	VkDescriptorPool m_descriptorPool;
	VkDescriptorSet m_descriptorSet;
};
}
