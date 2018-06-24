#pragma once

#include <array>
#include <string>
#include <vector>
#include "Textures/Texture.hpp"
#include "Renderer/Buffers/CommandBuffer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "IPipeline.hpp"
#include "PipelineCreate.hpp"
#include "ShaderProgram.hpp"

namespace fl
{
	class DepthStencil;

	/// <summary>
	/// Class that represents a Vulkan pipeline.
	/// </summary>
	class FL_EXPORT Pipeline :
		public IPipeline
	{
	private:
		GraphicsStage m_graphicsStage;
		PipelineCreate m_pipelineCreate;
		std::vector<PipelineDefine> m_defines;
		ShaderProgram *m_shaderProgram;

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
	public:
		/// <summary>
		/// Creates a new pipeline.
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		/// <param name="pipelineCreate"> The pipelines creation info. </param>
		/// <param name="defines"> A list of names that will be added as a #define. </param>
		Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<PipelineDefine> &defines = {});

		/// <summary>
		/// Deconstructor for the pipeline.
		/// </summary>
		~Pipeline();

		void BindPipeline(const CommandBuffer &commandBuffer) const;

		PipelineCreate GetPipelineCreate() const { return m_pipelineCreate; }

		ShaderProgram *GetShaderProgram() const override { return m_shaderProgram; }

		GraphicsStage GetGraphicsStage() const { return m_graphicsStage; }

		DepthStencil *GetDepthStencil(const int &stage = -1) const;

		Texture *GetTexture(const unsigned int &i, const int &stage = -1) const;

		VkDescriptorSetLayout GetVkDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		VkDescriptorPool GetVkDescriptorPool() const override { return m_descriptorPool; }

		VkPipeline GetVkPipeline() const override { return m_pipeline; }

		VkPipelineLayout GetVkPipelineLayout() const override { return m_pipelineLayout; }
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
