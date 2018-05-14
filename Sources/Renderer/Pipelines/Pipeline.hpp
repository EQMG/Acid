#pragma once

#include <array>
#include <string>
#include <vector>
#include "Textures/Texture.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "PipelineCreate.hpp"
#include "ShaderProgram.hpp"

namespace fl
{
	class DepthStencil;

	class FL_EXPORT PipelineDefine
	{
	private:
		std::string m_name;
		std::string m_value;
	public:
		PipelineDefine(const std::string &name, const std::string &value) :
			m_name(name),
			m_value(value)
		{
		}

		std::string GetName() const { return m_name; }

		std::string GetValue() const { return m_value; }
	};

	/// <summary>
	/// Class that represents a Vulkan pipeline.
	/// </summary>
	class FL_EXPORT Pipeline
	{
	private:
		GraphicsStage m_graphicsStage;
		PipelineCreate m_pipelineCreateInfo;
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
		/// <param name="pipelineCreateInfo"> The pipelines creation info. </param>
		/// <param name="defines"> A list of names that will be added as a #define. </param>
		Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreateInfo, const std::vector<PipelineDefine> &defines = std::vector<PipelineDefine>());

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

		VkDescriptorSetLayout GetVkDescriptorSetLayout() const { return m_descriptorSetLayout; }

		VkDescriptorPool GetVkDescriptorPool() const { return m_descriptorPool; }

		VkPipeline GetVkPipeline() const { return m_pipeline; }

		VkPipelineLayout GetVkPipelineLayout() const { return m_pipelineLayout; }
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
