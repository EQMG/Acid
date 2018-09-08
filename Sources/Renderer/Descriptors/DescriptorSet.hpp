#pragma once

#include <vector>
#include "Renderer/Commands/CommandBuffer.hpp"
#include "Renderer/Pipelines/IPipeline.hpp"

namespace acid
{
	class IDescriptor;

	class ACID_EXPORT DescriptorSet
	{
	private:
		VkPipelineLayout m_pipelineLayout;
		VkPipelineBindPoint m_pipelineBindPoint;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;
	public:
		DescriptorSet(const IPipeline &pipeline);

		~DescriptorSet();

		void Update(const std::vector<IDescriptor *> &descriptors);

		void BindDescriptor(const CommandBuffer &commandBuffer);

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }
	};
}
