#pragma once

#include "Renderer/Commands/CommandBuffer.hpp"
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
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_workgroupSize;
		bool m_pushDescriptors;
		std::vector<ShaderDefine> m_defines;

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
		/// <param name="shaderStage"> The shader file that will be loaded. </param>
		/// <param name="width"> The width used in local_size_x. </param>
		/// <param name="height"> The height used in local_size_y. </param>
		/// <param name="workgroupSize"> The amount of workgroups to use. </param>
		/// <param name="pushDescriptors"> If no actual descriptor sets are allocated but instead pushed. </param>
		/// <param name="defines"> A list of defines added to the top of each shader. </param>
		explicit Compute(const std::string &shaderStage, const uint32_t &width, const uint32_t &height,
			const uint32_t &workgroupSize = 16, const bool &pushDescriptors = false, const std::vector<ShaderDefine> &defines = {});

		~Compute();

		std::string GetShaderStage() const { return m_shaderStage; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		uint32_t GetWorkgroupSize() const { return m_workgroupSize; }

		bool IsPushDescriptors() const override { return m_pushDescriptors; }

		std::vector<ShaderDefine> GetDefines() const { return m_defines; }

		bool CmdRender(const CommandBuffer &commandBuffer) const;

		ShaderProgram *GetShaderProgram() const override { return m_shaderProgram.get(); }

		VkDescriptorSetLayout GetDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const override { return m_descriptorPool; }

		VkPipeline GetPipeline() const override { return m_pipeline; }

		VkPipelineLayout GetPipelineLayout() const override { return m_pipelineLayout; }

		virtual VkPipelineBindPoint GetPipelineBindPoint() const override { return VK_PIPELINE_BIND_POINT_COMPUTE; }
	private:
		void CreateShaderProgram();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreatePipelineLayout();

		void CreatePipelineCompute();
	};
}
