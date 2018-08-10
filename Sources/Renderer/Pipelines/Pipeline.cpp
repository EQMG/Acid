#include "Pipeline.hpp"

#include <cassert>
#include "Display/Display.hpp"
#include "Helpers/FileSystem.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	const std::vector<VkDynamicState> DYNAMIC_STATES = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	Pipeline::Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate) :
		IPipeline(),
		m_graphicsStage(graphicsStage),
		m_pipelineCreate(pipelineCreate),
		m_shaderProgram(std::make_shared<ShaderProgram>(pipelineCreate.GetShaderStages().back())),
		m_modules(std::vector<VkShaderModule>()),
		m_stages(std::vector<VkPipelineShaderStageCreateInfo>()),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE),
		m_inputAssemblyState({}),
		m_rasterizationState({}),
		m_blendAttachmentStates({}),
		m_colourBlendState({}),
		m_depthStencilState({}),
		m_viewportState({}),
		m_multisampleState({}),
		m_dynamicState({}),
		m_tessellationState({})
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		CreateShaderProgram();
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreateAttributes();

		switch (pipelineCreate.GetMode())
		{
		case PIPELINE_MODE_POLYGON:
			CreatePipelinePolygon();
			break;
		case PIPELINE_MODE_POLYGON_NO_DEPTH:
			CreatePipelinePolygonNoDepth();
			break;
		case PIPELINE_MODE_MRT:
			CreatePipelineMrt();
			break;
		case PIPELINE_MODE_MRT_NO_DEPTH:
			CreatePipelineMrtNoDepth();
			break;
		default:
			assert(false);
			break;
		}

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
	//	fprintf(stdout, "%s", m_shaderProgram->ToString().c_str());
		fprintf(stdout, "Pipeline '%s' created in %fms\n", m_pipelineCreate.GetShaderStages().back().c_str(), debugEnd - debugStart);
#endif
	}

	Pipeline::~Pipeline()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Display::CheckVk(vkDeviceWaitIdle(logicalDevice));

		for (auto &shaderModule : m_modules)
		{
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}

		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
		vkDestroyPipeline(logicalDevice, m_pipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
	}

	DepthStencil *Pipeline::GetDepthStencil(const int &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetDepthStencil();
	}

	Texture *Pipeline::GetTexture(const uint32_t &i, const int &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetFramebuffers()->GetTexture(i);
	}

	void Pipeline::CreateShaderProgram()
	{
		std::stringstream defineBlock;
		defineBlock << "\n";

		for (auto &define : m_pipelineCreate.GetDefines())
		{
			defineBlock << "#define " << define.GetName() << " " << define.GetValue() << "\n";
		}

		for (auto &shaderStage : m_pipelineCreate.GetShaderStages())
		{
			if (!FileSystem::FileExists(shaderStage))
			{
				fprintf(stderr, "File does not exist: '%s'\n", shaderStage.c_str());
				throw std::runtime_error("Could not create pipeline, missing shader stage!");
			}

			auto fileLoaded = FileSystem::ReadTextFile(shaderStage);

			if (!fileLoaded.has_value())
			{
				continue;
			}

			auto shaderCode = ShaderProgram::InsertDefineBlock(fileLoaded.value(), defineBlock.str());
			shaderCode = ShaderProgram::ProcessIncludes(shaderCode);

			VkShaderStageFlagBits stageFlag = ShaderProgram::GetShaderStage(shaderStage);
			VkShaderModule shaderModule = m_shaderProgram->ProcessShader(shaderCode, stageFlag);

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
			pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = stageFlag;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main";

			m_modules.emplace_back(shaderModule);
			m_stages.emplace_back(pipelineShaderStageCreateInfo);
		}

		m_shaderProgram->ProcessShader();
	}

	void Pipeline::CreateDescriptorLayout()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorSetLayoutBinding> bindings = std::vector<VkDescriptorSetLayoutBinding>();

		for (auto &type : m_shaderProgram->GetDescriptors())
		{
			bindings.emplace_back(type.GetLayoutBinding());
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();

		Display::CheckVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void Pipeline::CreateDescriptorPool()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorPoolSize> poolSizes = std::vector<VkDescriptorPoolSize>();

		for (auto &type : m_shaderProgram->GetDescriptors())
		{
			poolSizes.emplace_back(type.GetPoolSize());
		}

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
		descriptorPoolCreateInfo.maxSets = 4098; // Arbitrary number.

		Display::CheckVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
	}

	void Pipeline::CreatePipelineLayout()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

	//	VkPushConstantRange pushConstantRange = {};
	//	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	//	pushConstantRange.offset = sizeof(pushConstants);
	//	pushConstantRange.size = 0;

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
	//	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	//	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

		Display::CheckVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
	}

	void Pipeline::CreateAttributes()
	{
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_inputAssemblyState.primitiveRestartEnable = VK_FALSE;

		m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_rasterizationState.depthClampEnable = VK_FALSE;
		m_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		m_rasterizationState.polygonMode = m_pipelineCreate.GetPolygonMode();
		m_rasterizationState.cullMode = m_pipelineCreate.GetCullMode();
		m_rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		m_rasterizationState.depthBiasEnable = VK_FALSE;
		m_rasterizationState.depthBiasConstantFactor = 0.0f;
		m_rasterizationState.depthBiasClamp = 0.0f;
		m_rasterizationState.depthBiasSlopeFactor = 0.0f;
		m_rasterizationState.lineWidth = 1.0f;

		m_blendAttachmentStates[0].blendEnable = VK_TRUE;
		m_blendAttachmentStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		m_blendAttachmentStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		m_blendAttachmentStates[0].colorBlendOp = VK_BLEND_OP_ADD;
		m_blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		m_blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		m_blendAttachmentStates[0].alphaBlendOp = VK_BLEND_OP_ADD;
		m_blendAttachmentStates[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		m_colourBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_colourBlendState.logicOpEnable = VK_FALSE;
		m_colourBlendState.logicOp = VK_LOGIC_OP_COPY;
		m_colourBlendState.attachmentCount = static_cast<uint32_t>(m_blendAttachmentStates.size());
		m_colourBlendState.pAttachments = m_blendAttachmentStates.data();
		m_colourBlendState.blendConstants[0] = 0.0f;
		m_colourBlendState.blendConstants[1] = 0.0f;
		m_colourBlendState.blendConstants[2] = 0.0f;
		m_colourBlendState.blendConstants[3] = 0.0f;

		VkStencilOpState stencilOpState = {};
		stencilOpState.failOp = VK_STENCIL_OP_KEEP;
		stencilOpState.passOp = VK_STENCIL_OP_KEEP;
		stencilOpState.depthFailOp = VK_STENCIL_OP_KEEP;
		stencilOpState.compareOp = VK_COMPARE_OP_ALWAYS;
		stencilOpState.compareMask = 0b00000000;
		stencilOpState.writeMask = 0b11111111;
		stencilOpState.reference = 0b00000000;

		m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_depthStencilState.depthTestEnable = VK_TRUE;
		m_depthStencilState.depthWriteEnable = VK_TRUE;
		m_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
		m_depthStencilState.depthBoundsTestEnable = VK_FALSE;
		m_depthStencilState.stencilTestEnable = VK_FALSE;
		m_depthStencilState.front = stencilOpState;
		m_depthStencilState.back = stencilOpState;
		m_depthStencilState.minDepthBounds = 0.0f;
		m_depthStencilState.maxDepthBounds = 1.0f;

		m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_viewportState.viewportCount = 1;
		m_viewportState.scissorCount = 1;

		m_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_multisampleState.sampleShadingEnable = VK_FALSE;
		m_multisampleState.rasterizationSamples = Display::Get()->GetMsaaSamples();

		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.pDynamicStates = DYNAMIC_STATES.data();
		m_dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());

		m_tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		m_tessellationState.patchControlPoints = 3;
	}

	void Pipeline::CreatePipelinePolygon()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetPipelineCache();
		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());

		auto bindingDescriptions = m_pipelineCreate.GetVertexInput().GetBindingDescriptions();
		auto attributeDescriptions = m_pipelineCreate.GetVertexInput().GetAttributeDescriptions(); // m_shaderProgram->GetAttributeDescriptions()

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.renderPass = renderStage->GetRenderpass()->GetRenderpass();
		pipelineCreateInfo.subpass = m_graphicsStage.GetSubpass();
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;

		pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyState;
		pipelineCreateInfo.pRasterizationState = &m_rasterizationState;
		pipelineCreateInfo.pColorBlendState = &m_colourBlendState;
		pipelineCreateInfo.pMultisampleState = &m_multisampleState;
		pipelineCreateInfo.pViewportState = &m_viewportState;
		pipelineCreateInfo.pDepthStencilState = &m_depthStencilState;
		pipelineCreateInfo.pDynamicState = &m_dynamicState;
		pipelineCreateInfo.pTessellationState = &m_tessellationState;

		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
		pipelineCreateInfo.pStages = m_stages.data();

		// Create the graphics pipeline.
		Display::CheckVk(vkCreateGraphicsPipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreatePipelinePolygonNoDepth()
	{
		m_depthStencilState.depthTestEnable = VK_FALSE;
		m_depthStencilState.depthWriteEnable = VK_FALSE;

		CreatePipelinePolygon();
	}

	void Pipeline::CreatePipelineMrt()
	{
		std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates = {};

		for (uint32_t i = 0; i < Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass())->GetImageAttachments(); i++)
		{
			VkPipelineColorBlendAttachmentState blendAttachmentState = {};
			blendAttachmentState.blendEnable = VK_FALSE; // TODO: Configurable alpha blending.
			blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			blendAttachmentStates.emplace_back(blendAttachmentState);
		}

		m_colourBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
		m_colourBlendState.pAttachments = blendAttachmentStates.data();

		CreatePipelinePolygon();
	}

	void Pipeline::CreatePipelineMrtNoDepth()
	{
		std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates = {};

		for (uint32_t i = 0; i < Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass())->GetImageAttachments(); i++)
		{
			VkPipelineColorBlendAttachmentState blendAttachmentState = {};
			blendAttachmentState.blendEnable = VK_FALSE;
			blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			blendAttachmentStates.emplace_back(blendAttachmentState);
		}

		m_colourBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
		m_colourBlendState.pAttachments = blendAttachmentStates.data();

		m_depthStencilState.depthTestEnable = VK_FALSE;
		m_depthStencilState.depthWriteEnable = VK_FALSE;

		CreatePipelinePolygon();
	}
}
