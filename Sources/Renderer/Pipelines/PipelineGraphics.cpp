#include "PipelineGraphics.hpp"

#include "Renderer/Renderer.hpp"
#include "Files/FileSystem.hpp"

namespace acid
{
const std::vector<VkDynamicState> DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };

PipelineGraphics::PipelineGraphics(Stage stage, std::vector<std::string> shaderStages, std::vector<Shader::VertexInput> vertexInputs, std::vector<Shader::Define> defines,
	const Mode &mode, const Depth &depth, const VkPrimitiveTopology &topology, const VkPolygonMode &polygonMode, const VkCullModeFlags &cullMode, const VkFrontFace &frontFace,
	const bool &pushDescriptors) :
	m_stage(std::move(stage)),
	m_shaderStages(std::move(shaderStages)),
	m_vertexInputs(std::move(vertexInputs)),
	m_defines(std::move(defines)),
	m_mode(mode),
	m_depth(depth),
	m_topology(topology),
	m_polygonMode(polygonMode),
	m_cullMode(cullMode),
	m_frontFace(frontFace),
	m_pushDescriptors(pushDescriptors),
	m_shader(std::make_unique<Shader>(m_shaderStages.back())),
	m_dynamicStates(std::vector<VkDynamicState>(DYNAMIC_STATES)),
	m_descriptorSetLayout(VK_NULL_HANDLE),
	m_descriptorPool(VK_NULL_HANDLE),
	m_pipeline(VK_NULL_HANDLE),
	m_pipelineLayout(VK_NULL_HANDLE),
	m_pipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
	m_vertexInputStateCreateInfo({}),
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
#if defined(ACID_VERBOSE)
	auto debugStart = Engine::GetTime();
#endif

	std::sort(m_vertexInputs.begin(), m_vertexInputs.end());
	CreateShaderProgram();
	CreateDescriptorLayout();
	CreateDescriptorPool();
	CreatePipelineLayout();
	CreateAttributes();

	switch (m_mode)
	{
	case Mode::Polygon:
		CreatePipelinePolygon();
		break;
	case Mode::Mrt:
		CreatePipelineMrt();
		break;
	default:
		throw std::runtime_error("Unknown pipeline mode");
		break;
	}

#if defined(ACID_VERBOSE)
	auto debugEnd = Engine::GetTime();
	//Log::Out("%s\n", m_shader->ToString().c_str());
	Log::Out("Pipeline graphics '%s' created in %.3fms\n", m_shaderStages.back().c_str(), (debugEnd - debugStart).AsMilliseconds<float>());
#endif
}

PipelineGraphics::~PipelineGraphics()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	for (const auto &shaderModule : m_modules)
	{
		vkDestroyShaderModule(logicalDevice->GetLogicalDevice(), shaderModule, nullptr);
	}

	vkDestroyDescriptorPool(logicalDevice->GetLogicalDevice(), m_descriptorPool, nullptr);
	vkDestroyPipeline(logicalDevice->GetLogicalDevice(), m_pipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice->GetLogicalDevice(), m_pipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(logicalDevice->GetLogicalDevice(), m_descriptorSetLayout, nullptr);
}

const ImageDepth *PipelineGraphics::GetDepthStencil(const std::optional<uint32_t> &stage) const
{
	return Renderer::Get()->GetRenderStage(stage ? *stage : m_stage.first)->GetDepthStencil();
}

const Image2d *PipelineGraphics::GetTexture(const uint32_t &index, const std::optional<uint32_t> &stage) const
{
	return Renderer::Get()->GetRenderStage(stage ? *stage : m_stage.first)->GetFramebuffers()->GetAttachment(index);
}

Vector2ui PipelineGraphics::GetSize(const std::optional<uint32_t> &stage) const
{
	return Renderer::Get()->GetRenderStage(stage ? *stage : m_stage.first)->GetSize();
}

float PipelineGraphics::GetAspectRatio(const std::optional<uint32_t> &stage) const
{
	return Renderer::Get()->GetRenderStage(stage ? *stage : m_stage.first)->GetAspectRatio();
}

void PipelineGraphics::CreateShaderProgram()
{
	std::stringstream defineBlock;
	defineBlock << "\n";

	for (const auto &define : m_defines)
	{
		defineBlock << "#define " << define.first << " " << define.second << "\n";
	}

	for (const auto &shaderStage : m_shaderStages)
	{
		auto fileLoaded = Files::Read(shaderStage);

		if (!fileLoaded)
		{
			Log::Error("Shader Stage could not be loaded: '%s'\n", shaderStage.c_str());
			throw std::runtime_error("Could not create pipeline, missing shader stage");
			return;
		}

		auto shaderCode = Shader::InsertDefineBlock(*fileLoaded, defineBlock.str());
		shaderCode = Shader::ProcessIncludes(shaderCode);

		auto stageFlag = Shader::GetShaderStage(shaderStage);
		auto shaderModule = m_shader->ProcessShader(shaderCode, stageFlag);

		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
		pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineShaderStageCreateInfo.stage = stageFlag;
		pipelineShaderStageCreateInfo.module = shaderModule;
		pipelineShaderStageCreateInfo.pName = "main";
		m_stages.emplace_back(pipelineShaderStageCreateInfo);
		m_modules.emplace_back(shaderModule);
	}

	m_shader->ProcessShader();
}

void PipelineGraphics::CreateDescriptorLayout()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	auto &descriptorSetLayouts = m_shader->GetDescriptorSetLayouts();

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.flags = m_pushDescriptors ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;
	descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayouts.data();
	Renderer::CheckVk(vkCreateDescriptorSetLayout(logicalDevice->GetLogicalDevice(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
}

void PipelineGraphics::CreateDescriptorPool()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	auto descriptorPools = m_shader->GetDescriptorPools();

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.maxSets = 8192; // 16384;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
	descriptorPoolCreateInfo.pPoolSizes = descriptorPools.data();
	Renderer::CheckVk(vkCreateDescriptorPool(logicalDevice->GetLogicalDevice(), &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
}

void PipelineGraphics::CreatePipelineLayout()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	auto pushConstantRanges = m_shader->GetPushConstantRanges();

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
	pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
	Renderer::CheckVk(vkCreatePipelineLayout(logicalDevice->GetLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
}

void PipelineGraphics::CreateAttributes()
{
	auto physicalDevice = Renderer::Get()->GetPhysicalDevice();
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	if (m_polygonMode == VK_POLYGON_MODE_LINE && !logicalDevice->GetEnabledFeatures().fillModeNonSolid)
	{
		throw std::runtime_error("Cannot create graphics pipeline with line polygon mode when logical device does not support non solid fills.");
	}

	m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_inputAssemblyState.topology = m_topology;
	m_inputAssemblyState.primitiveRestartEnable = VK_FALSE;

	m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_rasterizationState.depthClampEnable = VK_FALSE;
	m_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	m_rasterizationState.polygonMode = m_polygonMode;
	m_rasterizationState.cullMode = m_cullMode;
	m_rasterizationState.frontFace = m_frontFace;
	m_rasterizationState.depthBiasEnable = VK_FALSE;
	//m_rasterizationState.depthBiasConstantFactor = 0.0f;
	//m_rasterizationState.depthBiasClamp = 0.0f;
	//m_rasterizationState.depthBiasSlopeFactor = 0.0f;
	m_rasterizationState.lineWidth = 1.0f;

	m_blendAttachmentStates[0].blendEnable = VK_TRUE;
	m_blendAttachmentStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	m_blendAttachmentStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	m_blendAttachmentStates[0].colorBlendOp = VK_BLEND_OP_ADD;
	m_blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	m_blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
	m_blendAttachmentStates[0].alphaBlendOp = VK_BLEND_OP_MAX;
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

	m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	m_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	m_depthStencilState.front = m_depthStencilState.back;
	m_depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;

	switch (m_depth)
	{
	case Depth::None:
		m_depthStencilState.depthTestEnable = VK_FALSE;
		m_depthStencilState.depthWriteEnable = VK_FALSE;
		break;
	case Depth::Read:
		m_depthStencilState.depthTestEnable = VK_TRUE;
		m_depthStencilState.depthWriteEnable = VK_FALSE;
		break;
	case Depth::Write:
		m_depthStencilState.depthTestEnable = VK_FALSE;
		m_depthStencilState.depthWriteEnable = VK_TRUE;
		break;
	case Depth::ReadWrite:
		m_depthStencilState.depthTestEnable = VK_TRUE;
		m_depthStencilState.depthWriteEnable = VK_TRUE;
		break;
	}

	m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_viewportState.viewportCount = 1;
	m_viewportState.scissorCount = 1;

	auto renderStage = Renderer::Get()->GetRenderStage(m_stage.first);
	bool multisampled = renderStage->IsMultisampled(m_stage.second);

	m_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_multisampleState.rasterizationSamples = multisampled ? physicalDevice->GetMsaaSamples() : VK_SAMPLE_COUNT_1_BIT;
	m_multisampleState.sampleShadingEnable = VK_FALSE;

	m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	m_dynamicState.dynamicStateCount = static_cast<uint32_t>(m_dynamicStates.size());
	m_dynamicState.pDynamicStates = m_dynamicStates.data();

	m_tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	m_tessellationState.patchControlPoints = 3;
}

void PipelineGraphics::CreatePipeline()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();
	auto pipelineCache = Renderer::Get()->GetPipelineCache();
	auto renderStage = Renderer::Get()->GetRenderStage(m_stage.first);

	std::vector<VkVertexInputBindingDescription> bindingDescriptions;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	uint32_t lastAttribute = 0;

	for (const auto &vertexInput : m_vertexInputs)
	{
		for (const auto &binding : vertexInput.GetBindingDescriptions())
		{
			bindingDescriptions.emplace_back(binding);
		}

		for (const auto &attribute : vertexInput.GetAttributeDescriptions())
		{
			/*bool shaderContains = false;

			for (const auto &[shaderAttributeName, shaderAttribute] : m_shader->GetAttributes())
			{
				if (attribute.location + lastAttribute == shaderAttribute->GetLocation())
				{
					shaderContains = true;
					break;
				}
			}

			if (!shaderContains)
			{
				continue;
			}*/

			auto &newAttribute = attributeDescriptions.emplace_back(attribute);
			newAttribute.location += lastAttribute;
		}

		if (!vertexInput.GetAttributeDescriptions().empty())
		{
			lastAttribute = attributeDescriptions.back().location + 1;
		}
	}

	m_vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
	m_vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	m_vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	m_vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
	pipelineCreateInfo.pStages = m_stages.data();

	pipelineCreateInfo.pVertexInputState = &m_vertexInputStateCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyState;
	pipelineCreateInfo.pTessellationState = &m_tessellationState;
	pipelineCreateInfo.pViewportState = &m_viewportState;
	pipelineCreateInfo.pRasterizationState = &m_rasterizationState;
	pipelineCreateInfo.pMultisampleState = &m_multisampleState;
	pipelineCreateInfo.pDepthStencilState = &m_depthStencilState;
	pipelineCreateInfo.pColorBlendState = &m_colourBlendState;
	pipelineCreateInfo.pDynamicState = &m_dynamicState;

	pipelineCreateInfo.layout = m_pipelineLayout;
	pipelineCreateInfo.renderPass = renderStage->GetRenderpass()->GetRenderpass();
	pipelineCreateInfo.subpass = m_stage.second;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;
	Renderer::CheckVk(vkCreateGraphicsPipelines(logicalDevice->GetLogicalDevice(), pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
}

void PipelineGraphics::CreatePipelinePolygon()
{
	CreatePipeline();
}

void PipelineGraphics::CreatePipelineMrt()
{
	auto renderStage = Renderer::Get()->GetRenderStage(m_stage.first);
	uint32_t attachmentCount = renderStage->GetAttachmentCount(m_stage.second);

	std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates;
	blendAttachmentStates.reserve(attachmentCount);

	for (uint32_t i = 0; i < attachmentCount; i++)
	{
		VkPipelineColorBlendAttachmentState blendAttachmentState = {};
		blendAttachmentState.blendEnable = VK_TRUE;
		blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
		blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blendAttachmentStates.emplace_back(blendAttachmentState);
	}

	m_colourBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
	m_colourBlendState.pAttachments = blendAttachmentStates.data();

	CreatePipeline();
}
}
