#pragma once

#include <array>
#include <string>

#include "../../platforms/glfw/Platform.hpp"
#include "../../shaders/Shader.hpp"

#include "Descriptor.hpp"
#include "InputState.hpp"

namespace Flounder
{
	enum PipelineType
	{
		PipelinePolygon,
		PipelineNoDepthTest,
		PipelineMrt,
		PipelineMultiTexture
	};

	/// <summary>
	/// Class that represents a Vulkan pipeline.
	/// </summary>
	class Pipeline
	{
	private:
		static const std::vector<VkDynamicState> DYNAMIC_STATES;

		std::string m_name;
		PipelineType m_pipelineType;

		Shader *m_shader;

		InputState m_inputState;
		Descriptor m_descriptor;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;

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
		/// <param name="name"> The pipelines name. </param>
		Pipeline(const std::string &name, const PipelineType &pipelineType, Shader *shader, const InputState &inputState, const Descriptor &descriptor);

		/// <summary>
		/// Deconstructor for the pipeline.
		/// </summary>
		~Pipeline();

		/// <summary>
		/// Gets the loaded name for the pipeline.
		/// </summary>
		/// <returns> The pipelines name. </returns>
		std::string GetName() const { return m_name; }

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

		VkPipeline GetPipeline() const { return m_pipeline; }

		VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
	private:
		void CreateAttributes();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreateDescriptorSet();

		void CreatePipelineLayout();

		void CreatePolygonPipeline();

		void CreateNoDepthTestPipeline();

		void CreateMrtPipeline();

		void CreateMultiTexturePipeline();
	};
}
