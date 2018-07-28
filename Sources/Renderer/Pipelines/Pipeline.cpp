#include "Pipeline.hpp"

#include <SPIRV/GlslangToSpv.h>
#include "Helpers/FileSystem.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	const std::vector<VkDynamicState> DYNAMIC_STATES = {
		VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
	};

	Pipeline::Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<PipelineDefine> &defines) :
		IPipeline(),
		m_graphicsStage(graphicsStage),
		m_pipelineCreate(pipelineCreate),
		m_defines(defines),
		m_shaderProgram(new ShaderProgram(pipelineCreate.GetShaderStages().back())),
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
		m_dynamicState({})
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
		case PIPELINE_MODE_COMPUTE:
			CreatePipelineCompute();
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
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		for (auto &shaderModule : m_modules)
		{
			vkDestroyShaderModule(logicalDevice, shaderModule, allocator);
		}

		delete m_shaderProgram;
		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, allocator);
		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, allocator);
		vkDestroyPipeline(logicalDevice, m_pipeline, allocator);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, allocator);
	}

	void Pipeline::BindPipeline(const CommandBuffer &commandBuffer) const
	{
		vkCmdBindPipeline(commandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
	}

	DepthStencil *Pipeline::GetDepthStencil(const int &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetDepthStencil();
	}

	Texture *Pipeline::GetTexture(const unsigned int &i, const int &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetFramebuffers()->GetTexture(i);
	}

	EShLanguage GetEshLanguage(const VkShaderStageFlagBits &stageFlag)
	{
		switch (stageFlag)
		{
		case VK_SHADER_STAGE_COMPUTE_BIT:
			return EShLangCompute;
		case VK_SHADER_STAGE_VERTEX_BIT:
			return EShLangVertex;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return EShLangTessControl;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return EShLangTessEvaluation;
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return EShLangGeometry;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return EShLangFragment;
		default:
			return EShLangCount;
		}
	}

	TBuiltInResource GetResources()
	{
		TBuiltInResource resources = {};
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 64;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxCombinedShaderOutputResources = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;
		resources.limits.nonInductiveForLoops = true;
		resources.limits.whileLoops = true;
		resources.limits.doWhileLoops = true;
		resources.limits.generalUniformIndexing = true;
		resources.limits.generalAttributeMatrixVectorIndexing = true;
		resources.limits.generalVaryingIndexing = true;
		resources.limits.generalSamplerIndexing = true;
		resources.limits.generalVariableIndexing = true;
		resources.limits.generalConstantMatrixVectorIndexing = true;
		return resources;
	}

	void Pipeline::CreateShaderProgram()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		std::stringstream defineBlock;
		defineBlock << "\n";

		for (auto &define : m_defines)
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

			auto shaderCode = ShaderProgram::InsertDefineBlock(FileSystem::ReadTextFile(shaderStage), defineBlock.str());

			VkShaderStageFlagBits stageFlag = ShaderProgram::GetShaderStage(shaderStage);
			EShLanguage language = GetEshLanguage(stageFlag);

			// Starts converting GLSL to SPIR-V.
			glslang::TShader shader = glslang::TShader(language);
			glslang::TProgram program;
			const char *shaderStrings[1];
			TBuiltInResource resources = GetResources();

			// Enable SPIR-V and Vulkan rules when parsing GLSL.
			EShMessages messages = (EShMessages) (EShMsgSpvRules | EShMsgVulkanRules);

			shaderStrings[0] = shaderCode.c_str();
			shader.setStrings(shaderStrings, 1);

			shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 100);
			shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
			shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

			//	if (shader.preprocess(&resources, 100, ENoProfile, false, false, messages, &str, includer))
			//	{
			//		fprintf(stderr, "SPRIV shader preprocess failed!\n");
			//	}

			if (!shader.parse(&resources, 100, false, messages))
			{
				fprintf(stdout, "%s\n", shader.getInfoLog());
				fprintf(stdout, "%s\n", shader.getInfoDebugLog());
				fprintf(stderr, "SPRIV shader compile failed!\n");
			}

			program.addShader(&shader);

			if (!program.link(messages) || !program.mapIO())
			{
				fprintf(stderr, "Error while linking shader program.\n");
			}

			program.buildReflection();
			//	program.dumpReflection();
			m_shaderProgram->LoadProgram(program, stageFlag);

			glslang::SpvOptions spvOptions;
			spvOptions.generateDebugInfo = true;
			spvOptions.disableOptimizer = true;
			spvOptions.optimizeSize = false;

			std::vector<uint32_t> spirv = std::vector<uint32_t>();
			glslang::GlslangToSpv(*program.getIntermediate(language), spirv, &spvOptions);

			VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
			shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderModuleCreateInfo.codeSize = spirv.size() * sizeof(uint32_t);
			shaderModuleCreateInfo.pCode = spirv.data();

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			Display::ErrorVk(vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, allocator, &shaderModule));

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
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		std::vector<VkDescriptorSetLayoutBinding> bindings = std::vector<VkDescriptorSetLayoutBinding>();

		for (auto &type : m_shaderProgram->GetDescriptors())
		{
			bindings.emplace_back(type.GetLayoutBinding());
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();

		vkDeviceWaitIdle(logicalDevice);
		Display::ErrorVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, allocator, &m_descriptorSetLayout));
	}

	void Pipeline::CreateDescriptorPool()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

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
		descriptorPoolCreateInfo.maxSets = 16384; // Arbitrary number.

		vkDeviceWaitIdle(logicalDevice);
		Display::ErrorVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, allocator, &m_descriptorPool));
	}

	void Pipeline::CreatePipelineLayout()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

		vkDeviceWaitIdle(logicalDevice);
		Display::ErrorVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, allocator, &m_pipelineLayout));
	}

	void Pipeline::CreateAttributes()
	{
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_inputAssemblyState.primitiveRestartEnable = VK_FALSE;

		m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_rasterizationState.depthClampEnable = VK_FALSE;
		m_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		m_rasterizationState.polygonMode = static_cast<VkPolygonMode>(m_pipelineCreate.GetPolygonMode());
		m_rasterizationState.cullMode = static_cast<VkCullModeFlags>(m_pipelineCreate.GetCullModeF());
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
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetVkPipelineCache();
		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());

		auto bindingDescriptions = m_pipelineCreate.GetVertexInput().GetBindingDescriptions();
		auto attributeDescriptions = m_pipelineCreate.GetVertexInput().GetAttributeDescriptions(); // m_shaderProgram->GetAttributeDescriptions()

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputStateCreateInfo.pVertexBindingDescriptions = m_pipelineCreate.GetVertexInput().GetBindingDescriptions().data();
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.renderPass = renderStage->GetRenderpass()->GetVkRenderpass();
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

		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
		pipelineCreateInfo.pStages = m_stages.data();

		// Create the graphics pipeline.
		Display::ErrorVk(vkCreateGraphicsPipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, allocator, &m_pipeline));
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

	void Pipeline::CreatePipelineCompute()
	{
		CreatePipelinePolygon();
	}
}
