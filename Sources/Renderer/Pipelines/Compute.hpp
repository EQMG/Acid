#pragma once

#include "Renderer/Commands/CommandBuffer.hpp"
#include "IPipeline.hpp"
#include "PipelineCreate.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a compute Vulkan pipeline.
	/// </summary>
	class ACID_EXPORT Compute :
		public IPipeline
	{
	private:
		ComputeCreate m_computeCreate;
		std::unique_ptr<ShaderProgram> m_shaderProgram;

		VkShaderModule m_shaderModule;
		VkPipelineShaderStageCreateInfo m_shaderStageCreateInfo;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	public:
		/// <summary>
		/// Creates a new compute pipeline.
		/// </summary>
		/// <param name="pipelineCreate"> The compute pipelines creation info. </param>
		Compute(const ComputeCreate &computeCreate);

		~Compute();

		ComputeCreate GetComputeCreate() const { return m_computeCreate; }

		void CmdRender(const CommandBuffer &commandBuffer) const;

		ShaderProgram *GetShaderProgram() const override { return m_shaderProgram.get(); }

		VkDescriptorSetLayout GetDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const override { return m_descriptorPool; }

		VkPipeline GetPipeline() const override { return m_pipeline; }

		VkPipelineLayout GetPipelineLayout() const override { return m_pipelineLayout; }

		virtual VkPipelineBindPoint GetPipelineBindPoint() const { return VK_PIPELINE_BIND_POINT_COMPUTE; }
	private:
		void CreateShaderProgram();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreatePipelineLayout();

		void CreatePipelineCompute();
	};
}
