#pragma once

#include <array>
#include <string>
#include <vector>
#include "../../Platforms/Platform.hpp"
#include "PipelineCreate.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a Vulkan pipeline.
	/// </summary>
	class Pipeline
	{
	private:
		int m_subpass;
		PipelineCreateInfo m_pipelineCreateInfo;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;

		std::vector<VkShaderModule> m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> m_stages;

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
		/// <param name="subpass"> The pipelines subpass. </param>
		/// <param name="pipelineCreateInfo"> The pipelines creation info. </param>
		Pipeline(const int &subpass, const PipelineCreateInfo &pipelineCreateInfo);

		/// <summary>
		/// Deconstructor for the pipeline.
		/// </summary>
		~Pipeline();

		void BindPipeline(const VkCommandBuffer &commandBuffer);

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

		VkPipeline GetPipeline() const { return m_pipeline; }

		VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
	private:
		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreateDescriptorSet();

		void CreatePipelineLayout();

		void CreateShaderStages();

		void CreateAttributes();

		void CreatePipelinePolygon();

		void CreatePipelinePolygonNoDepth();

		void CreatePipelineMrt();

		void CreatePipelineMrtNoDepth();
	};
}
