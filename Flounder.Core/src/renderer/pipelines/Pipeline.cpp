#include "Pipeline.hpp"

#include <cassert>
#include "../../devices/Display.hpp"
#include "../../helpers/HelperFile.hpp"
#include "../../helpers/HelperString.hpp"
#include "../Renderer.hpp"
#include "../RenderDeferred.hpp"

namespace Flounder
{
	const std::vector<VkDynamicState> Pipeline::DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	Pipeline::Pipeline(const std::string &name, const PipelineCreateInfo &pipelineCreateInfo) :
		m_name(name),
		m_pipelineCreateInfo(pipelineCreateInfo),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_descriptorSet(VK_NULL_HANDLE),
		m_modules(std::vector<VkShaderModule>()),
		m_stages(std::vector<VkPipelineShaderStageCreateInfo>()),
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
#if FLOUNDER_VERBOSE
		printf("Loading Pipeline: '%s'\n", name.c_str());
#endif
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreateDescriptorSet();
		CreatePipelineLayout();
		CreateShaderStages();
		CreateAttributes();

		switch (pipelineCreateInfo.pipelineModeFlags)
		{
		case PIPELINE_POLYGON:
			CreatePipelinePolygon();
			break;
		case PIPELINE_NO_DEPTH:
			CreatePipelineNoDepth();
			break;
		case PIPELINE_MRT:
			CreatePipelineMrt();
			break;
		case PIPELINE_MULTI_TEXTURE:
			CreatePipelineMultiTexture();
			break;
		default:
			assert(false);
			break;
		}
	}

	Pipeline::~Pipeline()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto shaderModule : m_modules)
		{
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}

		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
		vkDestroyPipeline(logicalDevice, m_pipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
	}

	void Pipeline::CreateDescriptorLayout()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorSetLayoutBinding> bindings = std::vector<VkDescriptorSetLayoutBinding>();

		for (auto type : m_pipelineCreateInfo.descriptors)
		{
			bindings.push_back(type.descriptorSetLayoutBinding);
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void Pipeline::CreateDescriptorPool()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorPoolSize> poolSizes = std::vector<VkDescriptorPoolSize>();

		for (auto type : m_pipelineCreateInfo.descriptors)
		{
			poolSizes.push_back(type.descriptorPoolSize);
		}

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
		descriptorPoolCreateInfo.maxSets = 50;

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
	}

	void Pipeline::CreateDescriptorSet()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = &m_descriptorSetLayout;

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &m_descriptorSet));
	}

	void Pipeline::CreatePipelineLayout()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

		vkDeviceWaitIdle(logicalDevice);
		Platform::ErrorVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
	}

	void Pipeline::CreateShaderStages()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto type : m_pipelineCreateInfo.shaderStages)
		{
			std::vector<char> shaderCode = HelperFile::ReadBinaryFile(type);
			VkShaderStageFlagBits stageFlag = VK_SHADER_STAGE_ALL;

			if (HelperString::Contains(type, "vert.spv"))
			{
				stageFlag = VK_SHADER_STAGE_VERTEX_BIT;
			}
			else if (HelperString::Contains(type, "tesc.spv"))
			{
				stageFlag = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			}
			else if (HelperString::Contains(type, "tese.spv"))
			{
				stageFlag = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			}
			else if (HelperString::Contains(type, "geom.spv"))
			{
				stageFlag = VK_SHADER_STAGE_GEOMETRY_BIT;
			}
			else if (HelperString::Contains(type, "frag.spv"))
			{
				stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
			}

			VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
			shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderModuleCreateInfo.codeSize = shaderCode.size();
			shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			Platform::ErrorVk(vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule));

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
			pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = stageFlag;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main"; // The shaders entry point.

			m_modules.push_back(shaderModule);
			m_stages.push_back(pipelineShaderStageCreateInfo);
		}
	}

	void Pipeline::CreateAttributes()
	{
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_inputAssemblyState.primitiveRestartEnable = VK_FALSE;

		m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_rasterizationState.depthClampEnable = VK_FALSE;
		m_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		m_rasterizationState.polygonMode = m_pipelineCreateInfo.polygonMode;
		m_rasterizationState.cullMode = m_pipelineCreateInfo.cullModeFlags;
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
		m_multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		m_multisampleState.minSampleShading = 0.0f;

		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.pDynamicStates = DYNAMIC_STATES.data();
		m_dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());
	}

	void Pipeline::CreatePipelinePolygon()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto renderPass = Renderer::Get()->GetRenderPass();
		const auto pipelineCache = Renderer::Get()->GetPipelineCache();

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_pipelineCreateInfo.vertexBindingDescriptions.size());
		vertexInputStateCreateInfo.pVertexBindingDescriptions = m_pipelineCreateInfo.vertexBindingDescriptions.data();
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_pipelineCreateInfo.vertexAttributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = m_pipelineCreateInfo.vertexAttributeDescriptions.data();

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
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
		pipelineCreateInfo.pStages = m_stages.data();

		// Create the graphics pipeline.
		Platform::ErrorVk(vkCreateGraphicsPipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreatePipelineNoDepth()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto renderPass = Renderer::Get()->GetRenderPass();
		const auto pipelineCache = Renderer::Get()->GetPipelineCache();

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_pipelineCreateInfo.vertexBindingDescriptions.size());
		vertexInputStateCreateInfo.pVertexBindingDescriptions = m_pipelineCreateInfo.vertexBindingDescriptions.data();
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_pipelineCreateInfo.vertexAttributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = m_pipelineCreateInfo.vertexAttributeDescriptions.data();

		m_depthStencilState.depthTestEnable = VK_FALSE;
		m_depthStencilState.depthWriteEnable = VK_FALSE;

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
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
		pipelineCreateInfo.pStages = m_stages.data();

		// Create the graphics pipeline.
		Platform::ErrorVk(vkCreateGraphicsPipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreatePipelineMrt()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto renderPass = Renderer::Get()->GetRenderPass();
		const auto pipelineCache = Renderer::Get()->GetPipelineCache();

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_pipelineCreateInfo.vertexBindingDescriptions.size());
		vertexInputStateCreateInfo.pVertexBindingDescriptions = m_pipelineCreateInfo.vertexBindingDescriptions.data();
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_pipelineCreateInfo.vertexAttributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = m_pipelineCreateInfo.vertexAttributeDescriptions.data();

		std::array<VkPipelineColorBlendAttachmentState, DeferredCount - 1> blendAttachmentStates = {};

		for (uint32_t i = 0; i < DeferredCount - 1; i++)
		{
			blendAttachmentStates[i].blendEnable = VK_FALSE;
			blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendAttachmentStates[i].colorBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendAttachmentStates[i].alphaBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentStates[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		}

		m_colourBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
		m_colourBlendState.pAttachments = blendAttachmentStates.data();

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
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
		pipelineCreateInfo.pStages = m_stages.data();

		// Create the graphics pipeline.
		Platform::ErrorVk(vkCreateGraphicsPipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreatePipelineMultiTexture()
	{
	}
}
