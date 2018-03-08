#pragma once

#include <string>
#include "../../Engine/Platform.hpp"
#include "../Pipelines/PipelineCreate.hpp"

namespace Flounder
{
	class Pipeline;

	class F_EXPORT DescriptorSet
	{
	private:
		VkDescriptorSet m_descriptorSet;

		std::vector<VkWriteDescriptorSet> m_writeSet;
	public:
		DescriptorSet(const Pipeline &pipeline);

		~DescriptorSet();

		void Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites);

		void BindDescriptor(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline);

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

	};
}
