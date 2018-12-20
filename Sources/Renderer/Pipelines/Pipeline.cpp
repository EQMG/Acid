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
		m_shaderProgram(std::make_unique<ShaderProgram>(pipelineCreate.GetShaderStages().back())),
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

		CreateShaderProgram();
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreateAttributes();

		switch (pipelineCreate.GetPipelineMode())
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
		Log::Out("Pipeline '%s' created in %ims\n", m_pipelineCreate.GetShaderStages().back().c_str(), (debugEnd - debugStart).AsMilliseconds());
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

		for (auto &define : m_pipelineCreate.GetDefines())
		{
			defineBlock << "#define " << define.GetName() << " " << define.GetValue() << "\n";
		}

		for (auto &shaderStage : m_pipelineCreate.GetShaderStages())
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

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = stageFlag,
				.module = shaderModule,
				.pName = "main"
			};
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

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = static_cast<uint32_t>(bindings.size()),
			.pBindings = bindings.data()
		};
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

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = 256, // TODO: Arbitrary number.
			.poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
			.pPoolSizes = poolSizes.data()
		};
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

			VkPushConstantRange pushConstantRange = {
				.stageFlags = uniformBlock->GetStageFlags(),
				.offset = currentOffset,
				.size = static_cast<uint32_t>(uniformBlock->GetSize())
			};
			pushConstantRanges.emplace_back(pushConstantRange);
			currentOffset += pushConstantRange.size;
		}

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 1,
			.pSetLayouts = &m_descriptorSetLayout,
			.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size()),
			.pPushConstantRanges = pushConstantRanges.data()
		};
		Display::CheckVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
	}

	void Pipeline::CreateAttributes()
	{
		m_inputAssemblyState = VkPipelineInputAssemblyStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE
		};

		m_rasterizationState = VkPipelineRasterizationStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = m_pipelineCreate.GetPolygonMode(),
			.cullMode = m_pipelineCreate.GetCullMode(),
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f
		};

		m_blendAttachmentStates[0] = VkPipelineColorBlendAttachmentState{
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		m_colourBlendState = VkPipelineColorBlendStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = static_cast<uint32_t>(m_blendAttachmentStates.size()),
			.pAttachments = m_blendAttachmentStates.data(),
			.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
		};

		m_depthStencilState = VkPipelineDepthStencilStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
			.front = m_depthStencilState.back,
			.back = {
				.compareOp = VK_COMPARE_OP_ALWAYS
			}
		};

		switch (m_pipelineCreate.GetPipelineDepth())
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

		m_viewportState = VkPipelineViewportStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.scissorCount = 1
		};

		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());
		bool multisampled = renderStage->IsMultisampled(m_graphicsStage.GetSubpass());

		m_multisampleState = VkPipelineMultisampleStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = multisampled ? Display::Get()->GetMsaaSamples() : VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE
		};

		m_dynamicState = VkPipelineDynamicStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast<uint32_t>(m_dynamicStates.size()),
			.pDynamicStates = m_dynamicStates.data()
		};

		m_tessellationState = VkPipelineTessellationStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
			.patchControlPoints = 3
		};
	}

	void Pipeline::CreatePipeline()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetPipelineCache();
		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());

		auto bindingDescriptions = std::vector<VkVertexInputBindingDescription>();
		auto attributeDescriptions = std::vector<VkVertexInputAttributeDescription>();
		uint32_t lastAttribute = 0;

		for (auto &vertexInput : m_pipelineCreate.GetVertexInputs())
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

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size()),
			.pVertexBindingDescriptions = bindingDescriptions.data(),
			.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
			.pVertexAttributeDescriptions = attributeDescriptions.data()
		};
		m_pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount = static_cast<uint32_t>(m_stages.size()),
			.pStages = m_stages.data(),

			.pVertexInputState = &vertexInputStateCreateInfo,
			.pInputAssemblyState = &m_inputAssemblyState,
			.pTessellationState = &m_tessellationState,
			.pViewportState = &m_viewportState,
			.pRasterizationState = &m_rasterizationState,
			.pMultisampleState = &m_multisampleState,
			.pDepthStencilState = &m_depthStencilState,
			.pColorBlendState = &m_colourBlendState,
			.pDynamicState = &m_dynamicState,

			.layout = m_pipelineLayout,
			.renderPass = renderStage->GetRenderpass()->GetRenderpass(),
			.subpass = m_graphicsStage.GetSubpass(),
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};
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
			VkPipelineColorBlendAttachmentState blendAttachmentState = {
				.blendEnable = VK_TRUE,
				.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
				.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				.colorBlendOp = VK_BLEND_OP_ADD,
				.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
				.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				.alphaBlendOp = VK_BLEND_OP_ADD,
				.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
			};
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

		VkComputePipelineCreateInfo pipelineCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
			.stage = m_stages[0],
			.layout = m_pipelineLayout,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};
		Display::CheckVk(vkCreateComputePipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));

		// Called when running the computer shader.
	//	vkCmdDispatch(commandBuffer.GetCommandBuffer(), Display::Get()->GetWidth() / 16, Display::Get()->GetHeight() / 16, 1);
	}
}
