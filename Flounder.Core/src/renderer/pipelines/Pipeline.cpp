#include "Pipeline.hpp"

namespace Flounder
{
	const std::vector<VkDynamicState> Pipeline::DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	Pipeline::Pipeline(const std::string &name, const PipelineType &pipelineType, const VertexInputState &vertexInputState, Shader *shader) :
		m_name(name),
		m_pipelineType(pipelineType),
		m_vertexInputState(vertexInputState),
		m_shader(shader),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE),
		m_inputAssemblyState({}),
		m_rasterizationState({}),
		m_blendAttachmentState({}),
		m_colourBlendState({}),
		m_depthStencilState({}),
		m_viewportState({}),
		m_multisampleState({}),
		m_dynamicState({})
	{
	}

	Pipeline::~Pipeline()
	{
	}

	void Pipeline::Create(const VkDevice *logicalDevice, VkRenderPass renderPass)
	{
		CreateAttributes();
		CreatePipelineLayout(logicalDevice);

		switch(m_pipelineType)
		{
			case PipelinePolygon:
				CreatePolygonPipeline(logicalDevice, renderPass);
				break;
			case PipelineNoDepthTest:
				CreateNoDepthTestPipeline(logicalDevice, renderPass);
				break;
			case PipelineMrt:
				CreateMrtPipeline(logicalDevice, renderPass);
				break;
			case PipelineMultiTexture:
				CreateMultiTexturePipeline(logicalDevice, renderPass);
				break;
			default:
				assert(false);
				break;
		}
	}

	void Pipeline::Cleanup(const VkDevice *logicalDevice)
	{
		vkDestroyPipeline(*logicalDevice, m_pipeline, nullptr);
		vkDestroyPipelineLayout(*logicalDevice, m_pipelineLayout, nullptr);
	}

	void Pipeline::CreateAttributes()
	{
		m_inputAssemblyState = {};
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_inputAssemblyState.primitiveRestartEnable = VK_FALSE;

		m_rasterizationState = {};
		m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_rasterizationState.depthClampEnable = VK_TRUE;
		m_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		m_rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		m_rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
		m_rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		m_rasterizationState.depthBiasEnable = VK_FALSE;
		m_rasterizationState.depthBiasConstantFactor = 0.000;
		m_rasterizationState.depthBiasClamp = 0.0f;
		m_rasterizationState.depthBiasSlopeFactor = 0.0f;
		m_rasterizationState.lineWidth = 1.0f;

		m_blendAttachmentState = {};
		m_blendAttachmentState.blendEnable = VK_TRUE;
		m_blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		m_blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		m_blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		m_blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		m_blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		m_blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
		m_blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		m_colourBlendState = {};
		m_colourBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_colourBlendState.logicOpEnable = VK_FALSE;
		m_colourBlendState.logicOp = VK_LOGIC_OP_COPY;
		m_colourBlendState.attachmentCount = 1;
		m_colourBlendState.pAttachments = &m_blendAttachmentState;

		m_depthStencilState = {};
		m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_depthStencilState.depthTestEnable = VK_TRUE;
		m_depthStencilState.depthWriteEnable = VK_TRUE;
		m_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		m_depthStencilState.front = m_depthStencilState.back;
		m_depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;

		m_viewportState = {};
		m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_viewportState.viewportCount = 1;
		m_viewportState.scissorCount = 1;

		m_multisampleState = {};
		m_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		m_multisampleState.minSampleShading = 0.0f;

		m_dynamicState = {};
		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.pDynamicStates = DYNAMIC_STATES.data();
		m_dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());
	}

	void Pipeline::CreatePipelineLayout(const VkDevice *logicalDevice)
	{
		// Pipeline layout struct.
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		// Creates the graphics pipeline layout.
		GlfwVulkan::ErrorVk(vkCreatePipelineLayout(*logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));
	}

	void Pipeline::CreatePolygonPipeline(const VkDevice *logicalDevice, VkRenderPass renderPass)
	{
		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.renderPass = renderPass;

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = m_vertexInputState.bindingDescriptionCount;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = m_vertexInputState.attributeDescriptionCount;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = m_vertexInputState.pVertexBindingDescriptions;
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = m_vertexInputState.pVertexAttributeDescriptions;

		pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyState;
		pipelineCreateInfo.pRasterizationState = &m_rasterizationState;
		pipelineCreateInfo.pColorBlendState = &m_colourBlendState;
		pipelineCreateInfo.pMultisampleState = &m_multisampleState;
		pipelineCreateInfo.pViewportState = &m_viewportState;
		pipelineCreateInfo.pDepthStencilState = &m_depthStencilState;
		pipelineCreateInfo.pDynamicState = &m_dynamicState;

		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_shader->GetStages()->size());
		pipelineCreateInfo.pStages = m_shader->GetStages()->data();

		// Create the graphics pipeline.
		GlfwVulkan::ErrorVk(vkCreateGraphicsPipelines(*logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreateNoDepthTestPipeline(const VkDevice *logicalDevice, VkRenderPass renderPass)
	{
	}

	void Pipeline::CreateMrtPipeline(const VkDevice *logicalDevice, VkRenderPass renderPass)
	{
	}

	void Pipeline::CreateMultiTexturePipeline(const VkDevice *logicalDevice, VkRenderPass renderPass)
	{
	}
}
