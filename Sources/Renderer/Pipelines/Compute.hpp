#pragma once

#include "Renderer/Buffers/CommandBuffer.hpp"
#include "IPipeline.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a compute Vulkan pipeline.
	/// </summary>
	class ACID_EXPORT Compute :
		public IPipeline
	{
	private:
		std::string m_shaderStage;
		std::shared_ptr<ShaderProgram> m_shaderProgram;

		VkShaderModule m_shaderModule;
		VkPipelineShaderStageCreateInfo m_shaderStageCreateInfo;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	public:
		Compute(const std::string &shaderStage);

		~Compute();

		void BindPipeline(const CommandBuffer &commandBuffer) const;

		void CmdRender(const CommandBuffer &commandBuffer, const uint32_t &width = 3200, const uint32_t &height = 2400, const uint32_t &workgroupSize = 32) const;

		std::shared_ptr<ShaderProgram> GetShaderProgram() const override { return m_shaderProgram; }

		VkDescriptorSetLayout GetVkDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		VkDescriptorPool GetVkDescriptorPool() const override { return m_descriptorPool; }

		VkPipeline GetVkPipeline() const override { return m_pipeline; }

		VkPipelineLayout GetVkPipelineLayout() const override { return m_pipelineLayout; }
	private:
		void CreateShaderProgram();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreatePipelineLayout();

		void CreatePipelineCompute();
	};
}
