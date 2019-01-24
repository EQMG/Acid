#pragma once

#include "Renderer/Commands/CommandBuffer.hpp"
#include "Pipeline.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a compute Vulkan compute pipeline.
	/// </summary>
	class ACID_EXPORT PipelineCompute :
		public Pipeline
	{
	public:
		/// <summary>
		/// Creates a new compute pipeline.
		/// </summary>
		/// <param name="shaderStage"> The shader file that will be loaded. </param>
		/// <param name="width"> The width used in local_size_x. </param>
		/// <param name="height"> The height used in local_size_y. </param>
		/// <param name="workgroupSize"> The amount of workgroups to use. </param>
		/// <param name="pushDescriptors"> If no actual descriptor sets are allocated but instead pushed. </param>
		/// <param name="defines"> A list of defines added to the top of each shader. </param>
		explicit PipelineCompute(const std::string &shaderStage, const uint32_t &width, const uint32_t &height,
			const uint32_t &workgroupSize = 16, const bool &pushDescriptors = false, const std::vector<Shader::Define> &defines = {});

		~PipelineCompute();

		const std::string &GetShaderStage() const { return m_shaderStage; }

		const uint32_t &GetWidth() const { return m_width; }

		const uint32_t &GetHeight() const { return m_height; }

		const uint32_t &GetWorkgroupSize() const { return m_workgroupSize; }

		const bool &IsPushDescriptors() const override { return m_pushDescriptors; }

		const std::vector<Shader::Define> &GetDefines() const { return m_defines; }

		bool CmdRender(const CommandBuffer &commandBuffer) const;

		const Shader *GetShaderProgram() const override { return m_shader.get(); }

		const VkDescriptorSetLayout &GetDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		const VkDescriptorPool &GetDescriptorPool() const override { return m_descriptorPool; }

		const VkPipeline &GetPipeline() const override { return m_pipeline; }

		const VkPipelineLayout &GetPipelineLayout() const override { return m_pipelineLayout; }

		const VkPipelineBindPoint &GetPipelineBindPoint() const override { return m_pipelineBindPoint; }
	private:
		void CreateShaderProgram();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreatePipelineLayout();

		void CreatePipelineCompute();

		std::string m_shaderStage;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_workgroupSize;
		bool m_pushDescriptors;
		std::vector<Shader::Define> m_defines;

		std::unique_ptr<Shader> m_shader;

		VkShaderModule m_shaderModule;
		VkPipelineShaderStageCreateInfo m_shaderStageCreateInfo;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
		VkPipelineBindPoint m_pipelineBindPoint;
	};
}
