﻿#pragma once

#include <array>
#include <string>
#include <vector>
#include "Textures/Texture.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "PipelineCreate.hpp"
#include "ShaderProgram.hpp"

namespace Flounder
{
	class DepthStencil;

	struct Define
	{
		std::string name;
		std::string value;
	};

	/// <summary>
	/// Class that represents a Vulkan pipeline.
	/// </summary>
	class F_EXPORT Pipeline
	{
	private:
		GraphicsStage m_graphicsStage;
		PipelineCreate m_pipelineCreateInfo;
		std::vector<Define> m_defines;
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
		/// <param name="pipelineCreateInfo"> The pipelines creation info. </param>
		/// <param name="defines"> A list of names that will be added as a #define. </param>
		Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreateInfo, const std::vector<Define> &defines = std::vector<Define>());

		/// <summary>
		/// Deconstructor for the pipeline.
		/// </summary>
		~Pipeline();

		void BindPipeline(const VkCommandBuffer &commandBuffer) const;

		PipelineCreate GetPipelineCreateInfo() const { return m_pipelineCreateInfo; }

		ShaderProgram *GetShaderProgram() const { return m_shaderProgram; }

		GraphicsStage GetGraphicsStage() const { return m_graphicsStage; }

		DepthStencil *GetDepthStencil(const int &stage = -1) const;

		Texture *GetTexture(const unsigned int &i, const int &stage = -1) const;

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }

		VkPipeline GetPipeline() const { return m_pipeline; }

		VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
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
	};
}
