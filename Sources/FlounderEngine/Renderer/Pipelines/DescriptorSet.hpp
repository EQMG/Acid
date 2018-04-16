#pragma once

#include <string>
#include <unordered_map>
#include "Display/Display.hpp"
#include "PipelineCreate.hpp"
#include "ShaderProgram.hpp"

namespace Flounder
{
	class Pipeline;

	class Descriptor;

	class F_EXPORT DescriptorSet
	{
	private:
		ShaderProgram *m_shaderProgram;
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;

		std::vector<Descriptor *> m_descriptors;
	public:
		DescriptorSet(const Pipeline &pipeline);

		~DescriptorSet();

		void Update(const std::vector<Descriptor *> &descriptors);

		void UpdateMap(const std::unordered_map<std::string, Descriptor *> &descriptorMap);

		void BindDescriptor(const VkCommandBuffer &commandBuffer);

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }
	};
}
