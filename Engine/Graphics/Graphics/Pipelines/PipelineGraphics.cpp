#include "PipelineGraphics.hpp"

#include "Graphics/Graphics.hpp"
#include "Files/Files.hpp"

namespace acid {
const std::vector<VkDynamicState> DYNAMIC_STATES = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH};

PipelineGraphics::PipelineGraphics(Stage stage, std::vector<std::filesystem::path> shaderStages, std::vector<Shader::VertexInput> vertexInputs, std::vector<Shader::Define> defines,
	Mode mode, Depth depth, VkPrimitiveTopology topology, VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, bool pushDescriptors) :
	stage(std::move(stage)),
	shaderStages(std::move(shaderStages)),
	vertexInputs(std::move(vertexInputs)),
	defines(std::move(defines)),
	mode(mode),
	depth(depth),
	topology(topology),
	polygonMode(polygonMode),
	cullMode(cullMode),
	frontFace(frontFace),
	pushDescriptors(pushDescriptors),
	shader(std::make_unique<Shader>()),
	dynamicStates(DYNAMIC_STATES),
	pipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	std::sort(this->vertexInputs.begin(), this->vertexInputs.end());
	CreateShaderProgram();
	CreateDescriptorLayout();
	CreateDescriptorPool();
	CreatePipelineLayout();
	CreateAttributes();

	switch (mode) {
	case Mode::Polygon:
		CreatePipelinePolygon();
		break;
	case Mode::MRT:
		CreatePipelineMrt();
		break;
	default:
		throw std::runtime_error("Unknown pipeline mode");
	}

#ifdef ACID_DEBUG
	Log::Out("Pipeline Graphics ", this->shaderStages.back(), " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

PipelineGraphics::~PipelineGraphics() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	for (const auto &shaderModule : modules)
		vkDestroyShaderModule(*logicalDevice, shaderModule, nullptr);

	vkDestroyDescriptorPool(*logicalDevice, descriptorPool, nullptr);
	vkDestroyPipeline(*logicalDevice, pipeline, nullptr);
	vkDestroyPipelineLayout(*logicalDevice, pipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(*logicalDevice, descriptorSetLayout, nullptr);
}

void PipelineGraphics::BindPipeline(const CommandBuffer &commandBuffer) const {
	vkCmdBindPipeline(commandBuffer, GetPipelineBindPoint(), GetPipeline());
}

const ImageDepth *PipelineGraphics::GetDepthStencil(const std::optional<uint32_t> &stage) const {
	return Graphics::Get()->GetRenderStage(stage ? *stage : this->stage.first)->GetDepthStencil();
}

const Image2d *PipelineGraphics::GetImage(uint32_t index, const std::optional<uint32_t> &stage) const {
	return Graphics::Get()->GetRenderStage(stage ? *stage : this->stage.first)->GetFramebuffers()->GetAttachment(index);
}

RenderArea PipelineGraphics::GetRenderArea(const std::optional<uint32_t> &stage) const {
	return Graphics::Get()->GetRenderStage(stage ? *stage : this->stage.first)->GetRenderArea();
}

void PipelineGraphics::CreateShaderProgram() {
	std::stringstream defineBlock;
	for (const auto &[defineName, defineValue] : defines)
		defineBlock << "#define " << defineName << " " << defineValue << '\n';

	for (const auto &shaderStage : shaderStages) {
		auto fileLoaded = Files::Read(shaderStage);

		if (!fileLoaded)
			throw std::runtime_error("Could not create pipeline, missing shader stage");

		auto stageFlag = Shader::GetShaderStage(shaderStage);
		auto shaderModule = shader->CreateShaderModule(shaderStage, *fileLoaded, defineBlock.str(), stageFlag);

		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
		pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineShaderStageCreateInfo.stage = stageFlag;
		pipelineShaderStageCreateInfo.module = shaderModule;
		pipelineShaderStageCreateInfo.pName = "main";
		stages.emplace_back(pipelineShaderStageCreateInfo);
		modules.emplace_back(shaderModule);
	}

	shader->CreateReflection();
}

void PipelineGraphics::CreateDescriptorLayout() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto &descriptorSetLayouts = shader->GetDescriptorSetLayouts();

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.flags = pushDescriptors ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;
	descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayouts.data();
	Graphics::CheckVk(vkCreateDescriptorSetLayout(*logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout));
}

void PipelineGraphics::CreateDescriptorPool() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto &descriptorPools = shader->GetDescriptorPools();

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.maxSets = 8192; // 16384;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
	descriptorPoolCreateInfo.pPoolSizes = descriptorPools.data();
	Graphics::CheckVk(vkCreateDescriptorPool(*logicalDevice, &descriptorPoolCreateInfo, nullptr, &descriptorPool));
}

void PipelineGraphics::CreatePipelineLayout() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto pushConstantRanges = shader->GetPushConstantRanges();

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
	pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
	Graphics::CheckVk(vkCreatePipelineLayout(*logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
}

void PipelineGraphics::CreateAttributes() {
	auto physicalDevice = Graphics::Get()->GetPhysicalDevice();
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	if (polygonMode == VK_POLYGON_MODE_LINE && !logicalDevice->GetEnabledFeatures().fillModeNonSolid) {
		throw std::runtime_error("Cannot create graphics pipeline with line polygon mode when logical device does not support non solid fills.");
	}

	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.topology = topology;
	inputAssemblyState.primitiveRestartEnable = VK_FALSE;

	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.depthClampEnable = VK_FALSE;
	rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	rasterizationState.polygonMode = polygonMode;
	rasterizationState.cullMode = cullMode;
	rasterizationState.frontFace = frontFace;
	rasterizationState.depthBiasEnable = VK_FALSE;
	//rasterizationState.depthBiasConstantFactor = 0.0f;
	//rasterizationState.depthBiasClamp = 0.0f;
	//rasterizationState.depthBiasSlopeFactor = 0.0f;
	rasterizationState.lineWidth = 1.0f;

	blendAttachmentStates[0].blendEnable = VK_TRUE;
	blendAttachmentStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blendAttachmentStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendAttachmentStates[0].colorBlendOp = VK_BLEND_OP_ADD;
	blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
	blendAttachmentStates[0].alphaBlendOp = VK_BLEND_OP_MAX;
	blendAttachmentStates[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	colourBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colourBlendState.logicOpEnable = VK_FALSE;
	colourBlendState.logicOp = VK_LOGIC_OP_COPY;
	colourBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
	colourBlendState.pAttachments = blendAttachmentStates.data();
	colourBlendState.blendConstants[0] = 0.0f;
	colourBlendState.blendConstants[1] = 0.0f;
	colourBlendState.blendConstants[2] = 0.0f;
	colourBlendState.blendConstants[3] = 0.0f;

	depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilState.front = depthStencilState.back;
	depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;

	switch (depth) {
	case Depth::None:
		depthStencilState.depthTestEnable = VK_FALSE;
		depthStencilState.depthWriteEnable = VK_FALSE;
		break;
	case Depth::Read:
		depthStencilState.depthTestEnable = VK_TRUE;
		depthStencilState.depthWriteEnable = VK_FALSE;
		break;
	case Depth::Write:
		depthStencilState.depthTestEnable = VK_FALSE;
		depthStencilState.depthWriteEnable = VK_TRUE;
		break;
	case Depth::ReadWrite:
		depthStencilState.depthTestEnable = VK_TRUE;
		depthStencilState.depthWriteEnable = VK_TRUE;
		break;
	}

	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	// TODO: Multisampled pipelines
	//auto renderStage = Graphics::Get()->GetRenderStage(stage.first);
	bool multisampled = false; // renderStage->IsMultisampled(stage.second);

	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.rasterizationSamples = multisampled ? physicalDevice->GetMsaaSamples() : VK_SAMPLE_COUNT_1_BIT;
	multisampleState.sampleShadingEnable = VK_FALSE;

	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	tessellationState.patchControlPoints = 3;
}

void PipelineGraphics::CreatePipeline() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();
	auto pipelineCache = Graphics::Get()->GetPipelineCache();
	auto renderStage = Graphics::Get()->GetRenderStage(stage.first);

	std::vector<VkVertexInputBindingDescription> bindingDescriptions;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	uint32_t lastAttribute = 0;

	for (const auto &vertexInput : vertexInputs) {
		for (const auto &binding : vertexInput.GetBindingDescriptions())
			bindingDescriptions.emplace_back(binding);

		for (const auto &attribute : vertexInput.GetAttributeDescriptions()) {
			/*bool shaderContains = false;

			for (const auto &[shaderAttributeName, shaderAttribute] : shader->GetAttributes()) {
				if (attribute.location + lastAttribute == shaderAttribute->GetLocation()) {
					shaderContains = true;
					break;
				}
			}

			if (!shaderContains)
				continue;*/

			auto &newAttribute = attributeDescriptions.emplace_back(attribute);
			newAttribute.location += lastAttribute;
		}

		if (!vertexInput.GetAttributeDescriptions().empty())
			lastAttribute = attributeDescriptions.back().location + 1;
	}

	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
	vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = static_cast<uint32_t>(stages.size());
	pipelineCreateInfo.pStages = stages.data();

	pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineCreateInfo.pTessellationState = &tessellationState;
	pipelineCreateInfo.pViewportState = &viewportState;
	pipelineCreateInfo.pRasterizationState = &rasterizationState;
	pipelineCreateInfo.pMultisampleState = &multisampleState;
	pipelineCreateInfo.pDepthStencilState = &depthStencilState;
	pipelineCreateInfo.pColorBlendState = &colourBlendState;
	pipelineCreateInfo.pDynamicState = &dynamicState;

	pipelineCreateInfo.layout = pipelineLayout;
	pipelineCreateInfo.renderPass = *renderStage->GetRenderpass();
	pipelineCreateInfo.subpass = stage.second;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;
	Graphics::CheckVk(vkCreateGraphicsPipelines(*logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &pipeline));
}

void PipelineGraphics::CreatePipelinePolygon() {
	CreatePipeline();
}

void PipelineGraphics::CreatePipelineMrt() {
	auto renderStage = Graphics::Get()->GetRenderStage(stage.first);
	auto attachmentCount = renderStage->GetAttachmentCount(stage.second);

	std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates;
	blendAttachmentStates.reserve(attachmentCount);

	for (uint32_t i = 0; i < attachmentCount; i++) {
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

	colourBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
	colourBlendState.pAttachments = blendAttachmentStates.data();

	CreatePipeline();
}
}
