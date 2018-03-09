#pragma once

#include <string>
#include "../../Engine/Platform.hpp"
#include "PipelineCreate.hpp"

namespace Flounder
{
	class Pipeline;
	class Descriptor;

	class F_EXPORT DescriptorSet
	{
	private:
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;

		std::vector<Descriptor*> m_descriptors;
	public:
		DescriptorSet(const Pipeline &pipeline);

		~DescriptorSet();

		void Update(const std::vector<Descriptor*> &descriptors);

		void BindDescriptor(const VkCommandBuffer &commandBuffer);

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

	};
}
