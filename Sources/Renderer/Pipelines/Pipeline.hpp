#pragma once

#include <array>
#include <string>
#include <vector>
#include "IPipeline.hpp"

namespace acid
{
	class DepthStencil;
	class Texture;

	enum PipelineMode
	{
		PIPELINE_MODE_POLYGON = 0,
		PIPELINE_MODE_MRT = 1,
		PIPELINE_MODE_COMPUTE = 2
	};

	enum PipelineDepth
	{
		PIPELINE_DEPTH_NONE = 0,
		PIPELINE_DEPTH_READ_WRITE = 1,
		PIPELINE_DEPTH_READ = 2,
		PIPELINE_DEPTH_WRITE = 3
	};


	class ACID_EXPORT PipelineCreate
	{
	private:
		friend class Pipeline;
		std::vector<std::string> m_shaderStages;
		std::vector<VertexInput> m_vertexInputs;

		PipelineMode m_pipelineMode;
		PipelineDepth m_depthMode;
		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullMode;

		std::vector<ShaderDefine> m_defines;
	public:
		PipelineCreate(const std::vector<std::string> &shaderStages, const std::vector<VertexInput> &vertexInputs, const PipelineMode &pipelineMode = PIPELINE_MODE_POLYGON, const PipelineDepth &depthMode = PIPELINE_DEPTH_READ_WRITE,
		               const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const std::vector<ShaderDefine> &defines = {}) :
			m_shaderStages(shaderStages),
			m_vertexInputs(vertexInputs),
			m_pipelineMode(pipelineMode),
			m_depthMode(depthMode),
			m_polygonMode(polygonMode),
			m_cullMode(cullMode),
			m_defines(defines)
		{
		}

		std::vector<std::string> GetShaderStages() const { return m_shaderStages; }

		std::vector<VertexInput> GetVertexInputs() const { return m_vertexInputs; }

		PipelineMode GetPipelineMode() const { return m_pipelineMode; }

		PipelineDepth GetDepthMode() const { return m_depthMode; }

		VkPolygonMode GetPolygonMode() const { return m_polygonMode; }

		VkCullModeFlags GetCullMode() const { return m_cullMode; }

		std::vector<ShaderDefine> GetDefines() const { return m_defines; }
	};

	/// <summary>
	/// Class that represents a Vulkan pipeline.
	/// </summary>
	class ACID_EXPORT Pipeline :
		public IPipeline
	{
	private:
		GraphicsStage m_graphicsStage;
		std::vector<std::string> m_shaderStages;
		std::vector<VertexInput> m_vertexInputs;
		PipelineMode m_pipelineMode;
		PipelineDepth m_depthMode;
		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullMode;
		std::vector<ShaderDefine> m_defines;

		std::unique_ptr<ShaderProgram> m_shaderProgram;

		std::vector<VkDynamicState> m_dynamicStates;

		std::vector<VkShaderModule> m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> m_stages;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
		VkPipelineBindPoint m_pipelineBindPoint;

		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
		VkPipelineRasterizationStateCreateInfo m_rasterizationState;
		std::array<VkPipelineColorBlendAttachmentState, 1> m_blendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo m_colourBlendState;
		VkPipelineDepthStencilStateCreateInfo m_depthStencilState;
		VkPipelineViewportStateCreateInfo m_viewportState;
		VkPipelineMultisampleStateCreateInfo m_multisampleState;
		VkPipelineDynamicStateCreateInfo m_dynamicState;
		VkPipelineTessellationStateCreateInfo m_tessellationState;
	public:
		/// <summary>
		/// Creates a new pipeline.
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		/// <param name="pipelineCreate"> The pipelines creation info. </param>
		Pipeline(const GraphicsStage &graphicsStage, const std::vector<std::string> &shaderStages, const std::vector<VertexInput> &vertexInputs, const PipelineMode &pipelineMode = PIPELINE_MODE_POLYGON, const PipelineDepth &depthMode = PIPELINE_DEPTH_READ_WRITE,
		         const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const std::vector<ShaderDefine> &defines = {});

		/// <summary>
		/// Creates a new pipeline.
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		/// <param name="pipelineCreate"> The pipelines creation info. </param>
		Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);

		~Pipeline();

		DepthStencil *GetDepthStencil(const int32_t &stage = -1) const;

		Texture *GetTexture(const uint32_t &index, const int32_t &stage = -1) const;

		uint32_t GetWidth(const int32_t &stage = -1) const;

		uint32_t GetHeight(const int32_t &stage = -1) const;

		GraphicsStage GetGraphicsStage() const { return m_graphicsStage; }

		std::vector<std::string> GetShaderStages() const { return m_shaderStages; }

		std::vector<VertexInput> GetVertexInputs() const { return m_vertexInputs; }

		PipelineMode GetPipelineMode() const { return m_pipelineMode; }

		PipelineDepth GetDepthMode() const { return m_depthMode; }

		VkPolygonMode GetPolygonMode() const { return m_polygonMode; }

		VkCullModeFlags GetCullMode() const { return m_cullMode; }

		std::vector<ShaderDefine> GetDefines() const { return m_defines; }

		ShaderProgram *GetShaderProgram() const override { return m_shaderProgram.get(); }

		VkDescriptorSetLayout GetDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const override { return m_descriptorPool; }

		VkPipeline GetPipeline() const override { return m_pipeline; }

		VkPipelineLayout GetPipelineLayout() const override { return m_pipelineLayout; }

		VkPipelineBindPoint GetPipelineBindPoint() const override { return m_pipelineBindPoint; }
	private:
		void CreateShaderProgram();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreatePipelineLayout();

		void CreateAttributes();

		void CreatePipeline();

		void CreatePipelinePolygon();

		void CreatePipelineMrt();

		void CreatePipelineCompute();
	};
}
