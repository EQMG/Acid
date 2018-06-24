#pragma once

#include "IPipeline.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a compute Vulkan pipeline.
	/// </summary>
	class FL_EXPORT Compute :
		public IPipeline
	{
	private:
		std::string m_shader;
		VertexInput m_vertexInput;

		ShaderProgram *m_shaderProgram;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	public:
		Compute(const std::string &shader, const VertexInput &vertexInput);

		~Compute();

		ShaderProgram *GetShaderProgram() const override { return m_shaderProgram; }

		VkDescriptorSetLayout GetVkDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		VkDescriptorPool GetVkDescriptorPool() const override { return m_descriptorPool; }

		VkPipeline GetVkPipeline() const override { return m_pipeline; }

		VkPipelineLayout GetVkPipelineLayout() const override { return m_pipelineLayout; }
	};
}
