#pragma once

#include <map>
#include "Renderer/Commands/CommandBuffer.hpp"
#include "Renderer/Pipelines/IPipeline.hpp"
#include "Renderer/Pipelines/ShaderProgram.hpp"

namespace acid
{
	class IDescriptor;

	class ACID_EXPORT DescriptorSet
	{
	private:
		std::shared_ptr<ShaderProgram> m_shaderProgram;
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
