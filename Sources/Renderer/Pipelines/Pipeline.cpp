#include "Pipeline.hpp"

#include <cassert>
#include <algorithm>
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

	Pipeline::Pipeline(const GraphicsStage &graphicsStage, const std::vector<std::string> &shaderStages, const std::vector<VertexInput> &vertexInputs, const PipelineMode &pipelineMode, const PipelineDepth &depthMode,
	    const VkPolygonMode &polygonMode, const VkCullModeFlags &cullMode, const std::vector<ShaderDefine> &defines) :
		IPipeline(),
		m_graphicsStage(graphicsStage),
		m_shaderStages(shaderStages),
		m_vertexInputs(vertexInputs),
		m_pipelineMode(pipelineMode),
		m_depthMode(depthMode),
		m_polygonMode(polygonMode),
		m_cullMode(cullMode),
		m_defines(defines),
		m_shaderProgram(std::make_unique<ShaderProgram>(m_shaderStages.back())),
		m_dynamicStates(std::vector<VkDynamicState>(DYNAMIC_STATES)),
		m_modules(std::vector<VkShaderModule>()),
		m_stages(std::vector<VkPipelineShaderStageCreateInfo>()),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE),
		m_pipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
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

		switch (m_pipelineMode)
		{
		case PIPELINE_MODE_POLYGON:
			CreatePipelinePolygon();
			break;
		case PIPELINE_MODE_MRT:
			CreatePipelineMrt();
			break;
		case PIPELINE_MODE_COMPUTE:
			CreatePipelineCompute();
			break;
		default:
			assert(false);
			break;
		}

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
	//	Log::Out("%s\n", m_shaderProgram->ToString().c_str());
		Log::Out("Pipeline '%s' created in %ims\n", m_shaderStages.back().c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	Pipeline::Pipeline(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate) :
		Pipeline(graphicsStage, pipelineCreate.m_shaderStages, pipelineCreate.m_vertexInputs, pipelineCreate.m_pipelineMode, pipelineCreate.m_depthMode,
			pipelineCreate.m_polygonMode, pipelineCreate.m_cullMode, pipelineCreate.m_defines)
	{
	}

	Pipeline::~Pipeline()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Display::CheckVk(vkDeviceWaitIdle(logicalDevice));

		for (auto &shaderModule : m_modules)
		{
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}

		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
		vkDestroyPipeline(logicalDevice, m_pipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, nullptr);
	}

	DepthStencil *Pipeline::GetDepthStencil(const int32_t &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetDepthStencil();
	}

	Texture *Pipeline::GetTexture(const uint32_t &index, const int32_t &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetFramebuffers()->GetAttachment(index);
	}

	uint32_t Pipeline::GetWidth(const int32_t &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetWidth();
	}

	uint32_t Pipeline::GetHeight(const int32_t &stage) const
	{
		return Renderer::Get()->GetRenderStage(stage == -1 ? m_graphicsStage.GetRenderpass() : stage)->GetHeight();
	}

	void Pipeline::CreateShaderProgram()
	{
		std::stringstream defineBlock;
		defineBlock << "\n";

		for (auto &define : m_defines)
		{
			defineBlock << "#define " << define.first << " " << define.second << "\n";
		}

		for (auto &shaderStage : m_shaderStages)
		{
			auto fileLoaded = Files::Read(shaderStage);

			if (!fileLoaded)
			{
				Log::Error("Shader Stage could not be loaded: '%s'\n", shaderStage.c_str());
				assert(false && "Could not create pipeline, missing shader stage!");
				return;
			}

			auto shaderCode = ShaderProgram::InsertDefineBlock(*fileLoaded, defineBlock.str());
			shaderCode = ShaderProgram::ProcessIncludes(shaderCode);

			VkShaderStageFlagBits stageFlag = ShaderProgram::GetShaderStage(shaderStage);
			VkShaderModule shaderModule = m_shaderProgram->ProcessShader(shaderCode, stageFlag);

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
			pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = stageFlag;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main";
			m_stages.emplace_back(pipelineShaderStageCreateInfo);
			m_modules.emplace_back(shaderModule);
		}

		m_shaderProgram->ProcessShader();
	}

	void Pipeline::CreateDescriptorLayout()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto descriptorSetLayouts = m_shaderProgram->GetDescriptorSetLayouts();

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayouts.data();
		Display::CheckVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void Pipeline::CreateDescriptorPool()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto descriptorPools = m_shaderProgram->GetDescriptorPools();

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.maxSets = 16384;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
		descriptorPoolCreateInfo.pPoolSizes = descriptorPools.data();
		Display::CheckVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
	}

	void Pipeline::CreatePipelineLayout()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkPushConstantRange> pushConstantRanges = {};
		uint32_t currentOffset = 0;

		for (auto &uniformBlock : m_shaderProgram->GetUniformBlocks())
		{
			if (uniformBlock->GetType() != BLOCK_PUSH)
			{
				continue;
			}

			VkPushConstantRange pushConstantRange = {};
			pushConstantRange.stageFlags = uniformBlock->GetStageFlags();
			pushConstantRange.offset = currentOffset;
			pushConstantRange.size = static_cast<uint32_t>(uniformBlock->GetSize());
			pushConstantRanges.emplace_back(pushConstantRange);
			currentOffset += pushConstantRange.size;
		}

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
		pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
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
		m_rasterizationState.polygonMode = m_polygonMode;
		m_rasterizationState.cullMode = m_cullMode;
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
		m_blendAttachmentStates[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

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

		switch (m_depthMode)
		{
			case PIPELINE_DEPTH_NONE:
				m_depthStencilState.depthTestEnable = VK_FALSE;
				m_depthStencilState.depthWriteEnable = VK_FALSE;
				break;
			case PIPELINE_DEPTH_READ_WRITE:
				m_depthStencilState.depthTestEnable = VK_TRUE;
				m_depthStencilState.depthWriteEnable = VK_TRUE;
				break;
			case PIPELINE_DEPTH_READ:
				m_depthStencilState.depthTestEnable = VK_TRUE;
				m_depthStencilState.depthWriteEnable = VK_FALSE;
				break;
			case PIPELINE_DEPTH_WRITE:
				m_depthStencilState.depthTestEnable = VK_FALSE;
				m_depthStencilState.depthWriteEnable = VK_TRUE;
				break;
		}

		m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_viewportState.viewportCount = 1;
		m_viewportState.scissorCount = 1;

		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());
		bool multisampled = renderStage->IsMultisampled(m_graphicsStage.GetSubpass());

		m_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_multisampleState.rasterizationSamples = multisampled ? Display::Get()->GetMsaaSamples() : VK_SAMPLE_COUNT_1_BIT;
		m_multisampleState.sampleShadingEnable = VK_FALSE;

		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.dynamicStateCount = static_cast<uint32_t>(m_dynamicStates.size());
		m_dynamicState.pDynamicStates = m_dynamicStates.data();

		m_tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		m_tessellationState.patchControlPoints = 3;
	}

	void Pipeline::CreatePipeline()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetPipelineCache();
		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());

		auto bindingDescriptions = std::vector<VkVertexInputBindingDescription>();
		auto attributeDescriptions = std::vector<VkVertexInputAttributeDescription>();
		uint32_t lastAttribute = 0;

		for (auto &vertexInput : m_vertexInputs)
		{
			for (auto &binding : vertexInput.GetBindingDescriptions())
			{
				bindingDescriptions.emplace_back(binding);
			}

			for (auto &attribute : vertexInput.GetAttributeDescriptions())
			{
				bool shaderContains = false;

				for (auto &shaderAttribute : m_shaderProgram->GetVertexAttributes())
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
				}

				auto &newAttribute = attributeDescriptions.emplace_back(attribute);
				newAttribute.location += lastAttribute;
			}

			if (!vertexInput.GetAttributeDescriptions().empty())
			{
				lastAttribute = attributeDescriptions.back().location + 1;
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		m_pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
		pipelineCreateInfo.pStages = m_stages.data();

		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
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
		pipelineCreateInfo.subpass = m_graphicsStage.GetSubpass();
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;
		Display::CheckVk(vkCreateGraphicsPipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreatePipelinePolygon()
	{
		CreatePipeline();
	}

	void Pipeline::CreatePipelineMrt()
	{
		std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates = {};

		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());
		uint32_t attachmentCount = renderStage->GetAttachmentCount(m_graphicsStage.GetSubpass());

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
			blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			blendAttachmentStates.emplace_back(blendAttachmentState);
		}

		m_colourBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
		m_colourBlendState.pAttachments = blendAttachmentStates.data();

		CreatePipeline();
	}

	void Pipeline::CreatePipelineCompute()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetPipelineCache();

		m_pipelineBindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;

		VkComputePipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stage = m_stages[0];
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;
		Display::CheckVk(vkCreateComputePipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));

		// Called when running the computer shader.
	//	vkCmdDispatch(commandBuffer.GetCommandBuffer(), Display::Get()->GetWidth() / 16, Display::Get()->GetHeight() / 16, 1);
	}
}
