#pragma once

#include <array>
#include <string>
#include <vector>
#include "Textures/Texture.hpp"
#include "PipelineCreate.hpp"
#include "ShaderProgram.hpp"

namespace acid
{
	class DepthStencil;

	/// <summary>
	/// Class that represents a Vulkan pipeline.
	/// </summary>
	class ACID_EXPORT Pipeline :
		public IPipeline
	{
	private:
		GraphicsStage m_graphicsStage;
		PipelineCreate m_pipelineCreate;
		std::shared_ptr<ShaderProgram> m_shaderProgram;

		std::vector<VkShaderModule> m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> m_stages;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;

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
		Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);

		/// <summary>
		/// Deconstructor for the pipeline.
		/// </summary>
		~Pipeline();

		PipelineCreate GetPipelineCreate() const { return m_pipelineCreate; }

		std::shared_ptr<ShaderProgram> GetShaderProgram() const override { return m_shaderProgram; }

		GraphicsStage GetGraphicsStage() const { return m_graphicsStage; }

		DepthStencil *GetDepthStencil(const int &stage = -1) const;

		Texture *GetTexture(const uint32_t &i, const int &stage = -1) const;

		VkDescriptorSetLayout GetDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const override { return m_descriptorPool; }

		VkPipeline GetPipeline() const override { return m_pipeline; }

		VkPipelineLayout GetPipelineLayout() const override { return m_pipelineLayout; }

		virtual VkPipelineBindPoint GetPipelineBindPoint() const { return VK_PIPELINE_BIND_POINT_GRAPHICS; }
	private:
		void CreateShaderProgram();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreatePipelineLayout();

		void CreateAttributes();

		void CreatePipelinePolygon();

		void CreatePipelinePolygonNoDepth();

		void CreatePipelineMrt();

		void CreatePipelineMrtNoDepth();

		void CreatePipelineCompute();
	};
}
