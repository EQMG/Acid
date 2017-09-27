#include "Pipeline.hpp"
#include "../../devices/Display.hpp"

namespace Flounder
{
	const std::vector<VkDynamicState> Pipeline::DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR }; 

	Pipeline::Pipeline(const std::string &name, const PipelineType &pipelineType, const Shader &shader) :
		m_name(name),
		m_pipelineType(pipelineType),
		m_shader(shader),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE),
		m_inputAssemblyState({}),
		m_rasterizationState({}),
		m_blendAttachmentStates({}),
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

	void Pipeline::Create(const VkDevice &logicalDevice, const VkRenderPass &renderPass, const VertexInputState &vertexInputState)
	{
		m_vertexInputState = vertexInputState;
		/*CreateAttributes();
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
		}*/

		// Vertex input struct.
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = m_vertexInputState.bindingDescriptionCount;
		vertexInputInfo.vertexAttributeDescriptionCount = m_vertexInputState.attributeDescriptionCount;
		vertexInputInfo.pVertexBindingDescriptions = m_vertexInputState.pVertexBindingDescriptions;
		vertexInputInfo.pVertexAttributeDescriptions = m_vertexInputState.pVertexAttributeDescriptions;

		// Input assembly struct.
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Viewport struct.
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(Display::Get()->GetWidth());
		viewport.height = static_cast<float>(Display::Get()->GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Scissor rect struct.
		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = Display::Get()->GetWidth();
		scissor.extent.height = Display::Get()->GetHeight();

		// Viewport state struct.
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// Rasterizer.
		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		// Multisampling struct.
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// Colour blending attachment struct.
		VkPipelineColorBlendAttachmentState colourBlendAttachment = {};
		colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colourBlendAttachment.blendEnable = VK_FALSE;

		// Colour blend struct.
		VkPipelineColorBlendStateCreateInfo colourBlending = {};
		colourBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colourBlending.logicOpEnable = VK_FALSE;
		colourBlending.logicOp = VK_LOGIC_OP_COPY;
		colourBlending.attachmentCount = 1;
		colourBlending.pAttachments = &colourBlendAttachment;
		colourBlending.blendConstants[0] = 0.0f;
		colourBlending.blendConstants[1] = 0.0f;
		colourBlending.blendConstants[2] = 0.0f;
		colourBlending.blendConstants[3] = 0.0f;

		VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.depthTestEnable = VK_TRUE;
		depthStencilState.depthWriteEnable = VK_TRUE;
		depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencilState.depthBoundsTestEnable = VK_FALSE;
		depthStencilState.stencilTestEnable = VK_FALSE;

		// Dynamic states struct.
		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_LINE_WIDTH };
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;

		// Pipeline layout struct.
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		// Creates the graphics pipeline layout.
		GlfwVulkan::ErrorVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

		// Pipeline info struct.
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(m_shader.GetStages()->size());
		pipelineInfo.pStages = m_shader.GetStages()->data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colourBlending;
		pipelineInfo.pDepthStencilState = &depthStencilState;
		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		// Create the graphics pipeline.
		GlfwVulkan::ErrorVk(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));
	}

	void Pipeline::Cleanup(const VkDevice &logicalDevice)
	{
		vkDestroyPipeline(logicalDevice, m_pipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
	}

	void Pipeline::CreateAttributes()
	{
		m_inputAssemblyState = {};
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_inputAssemblyState.primitiveRestartEnable = VK_FALSE;

		m_rasterizationState = {};
		m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_rasterizationState.depthClampEnable = VK_FALSE;
		m_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		m_rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		m_rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
		m_rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	//	m_rasterizationState.depthBiasEnable = VK_FALSE;
	//	m_rasterizationState.depthBiasConstantFactor = 0.0f;
	//	m_rasterizationState.depthBiasClamp = 0.0f;
	//	m_rasterizationState.depthBiasSlopeFactor = 0.0f;
		m_rasterizationState.lineWidth = 1.0f;

		m_blendAttachmentStates = {};
		m_blendAttachmentStates[0].blendEnable = VK_TRUE;
		m_blendAttachmentStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		m_blendAttachmentStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		m_blendAttachmentStates[0].colorBlendOp = VK_BLEND_OP_ADD;
		m_blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		m_blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		m_blendAttachmentStates[0].alphaBlendOp = VK_BLEND_OP_ADD;
		m_blendAttachmentStates[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		m_colourBlendState = {};
		m_colourBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_colourBlendState.logicOpEnable = VK_FALSE;
		m_colourBlendState.logicOp = VK_LOGIC_OP_COPY;
		m_colourBlendState.attachmentCount = static_cast<uint32_t>(m_blendAttachmentStates.size());
		m_colourBlendState.pAttachments = m_blendAttachmentStates.data();
		m_colourBlendState.blendConstants[0] = 0.0f;
		m_colourBlendState.blendConstants[1] = 0.0f;
		m_colourBlendState.blendConstants[2] = 0.0f;
		m_colourBlendState.blendConstants[3] = 0.0f;

		/*VkStencilOpState stencilOpState = {};
		stencilOpState.failOp = VK_STENCIL_OP_KEEP;
		stencilOpState.passOp = VK_STENCIL_OP_KEEP;
		stencilOpState.depthFailOp = VK_STENCIL_OP_KEEP;
		stencilOpState.compareOp = VK_COMPARE_OP_ALWAYS;
		stencilOpState.compareMask = 0b00000000;
		stencilOpState.writeMask = 0b11111111;
		stencilOpState.reference = 0b00000000;*/

		m_depthStencilState = {};
		m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_depthStencilState.depthTestEnable = VK_TRUE;
		m_depthStencilState.depthWriteEnable = VK_TRUE;
		m_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
		m_depthStencilState.depthBoundsTestEnable = VK_FALSE;
		m_depthStencilState.stencilTestEnable = VK_FALSE;
	//	m_depthStencilState.front = stencilOpState;
	//	m_depthStencilState.back = stencilOpState;
	//	m_depthStencilState.minDepthBounds = 0.0f;
	//	m_depthStencilState.maxDepthBounds = 1.0f;

		m_viewportState = {};
		m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_viewportState.viewportCount = 1;
		m_viewportState.scissorCount = 1;

		m_multisampleState = {};
		m_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_multisampleState.sampleShadingEnable = VK_FALSE;
		m_multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	//	m_multisampleState.minSampleShading = 0.0f;

		m_dynamicState = {};
		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.pDynamicStates = DYNAMIC_STATES.data();
		m_dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());
	}

	void Pipeline::CreatePipelineLayout(const VkDevice &logicalDevice)
	{
		// Pipeline layout struct.
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		// Creates the graphics pipeline layout.
		GlfwVulkan::ErrorVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));
	}

	void Pipeline::CreatePolygonPipeline(const VkDevice &logicalDevice, const VkRenderPass &renderPass)
	{
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = m_vertexInputState.bindingDescriptionCount;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = m_vertexInputState.attributeDescriptionCount;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = m_vertexInputState.pVertexBindingDescriptions;
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = m_vertexInputState.pVertexAttributeDescriptions;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.renderPass = renderPass;
		pipelineCreateInfo.subpass = 0;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;

		pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyState;
		pipelineCreateInfo.pRasterizationState = &m_rasterizationState;
		pipelineCreateInfo.pColorBlendState = &m_colourBlendState;
		pipelineCreateInfo.pMultisampleState = &m_multisampleState;
		pipelineCreateInfo.pViewportState = &m_viewportState;
		pipelineCreateInfo.pDepthStencilState = &m_depthStencilState;
		pipelineCreateInfo.pDynamicState = &m_dynamicState;

		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_shader.GetStages()->size());
		pipelineCreateInfo.pStages = m_shader.GetStages()->data();

		// Create the graphics pipeline.
		GlfwVulkan::ErrorVk(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreateNoDepthTestPipeline(const VkDevice &logicalDevice, const VkRenderPass &renderPass)
	{
	}

	void Pipeline::CreateMrtPipeline(const VkDevice &logicalDevice, const VkRenderPass &renderPass)
	{
	}

	void Pipeline::CreateMultiTexturePipeline(const VkDevice &logicalDevice, const VkRenderPass &renderPass)
	{
	}
}
