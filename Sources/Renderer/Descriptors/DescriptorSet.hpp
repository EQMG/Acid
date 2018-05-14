#pragma once

#include <string>
#include <map>
#include "Display/Display.hpp"
#include "Renderer/Pipelines/PipelineCreate.hpp"
#include "Renderer/Pipelines/ShaderProgram.hpp"

namespace fl
{
	class IDescriptor;

	class Pipeline;

	class FL_EXPORT DescriptorSet
	{
	private:
		ShaderProgram *m_shaderProgram;
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;
	public:
		DescriptorSet(const Pipeline &pipeline);

		~DescriptorSet();

		void Update(const std::vector<IDescriptor *> &descriptors);

		void BindDescriptor(const VkCommandBuffer &commandBuffer);

		VkDescriptorSet GetVkDescriptorSet() const { return m_descriptorSet; }
	};
}
