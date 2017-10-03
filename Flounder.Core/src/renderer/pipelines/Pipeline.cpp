#include "Pipeline.hpp"

#include <cassert>
#include "../../devices/Display.hpp"
#include "../Renderer.hpp"

namespace Flounder
{
	const std::vector<VkDynamicState> Pipeline::DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	Pipeline::Pipeline(const std::string &name, const PipelineType &pipelineType, Shader *shader, const std::vector<UniformBuffer*> &uniformBuffers) :
		m_texture("res/treeBirchSmall/diffuse.png"),

		m_name(name),
		m_pipelineType(pipelineType),

		m_shader(shader),
		m_uniformBuffers(std::vector<UniformBuffer*>(uniformBuffers)),
		m_descriptorPool(VK_NULL_HANDLE),
		m_descriptorSet(VK_NULL_HANDLE),

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

	void Pipeline::Create(const VertexInputState &vertexInputState)
	{
		m_vertexInputState = vertexInputState;
		m_texture.Create();
		CreateAttributes();
		CreateDescriptorPool();
		CreateDescriptorSet();
		CreatePipelineLayout();

		switch (m_pipelineType)
		{
		case PipelinePolygon:
			CreatePolygonPipeline();
			break;
		case PipelineNoDepthTest:
			CreateNoDepthTestPipeline();
			break;
		case PipelineMrt:
			CreateMrtPipeline();
			break;
		case PipelineMultiTexture:
			CreateMultiTexturePipeline();
			break;
		default:
			assert(false);
			break;
		}
	}

	void Pipeline::Cleanup()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		m_texture.Cleanup();

		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
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
		m_rasterizationState.depthBiasEnable = VK_FALSE;
		m_rasterizationState.depthBiasConstantFactor = 0.0f;
		m_rasterizationState.depthBiasClamp = 0.0f;
		m_rasterizationState.depthBiasSlopeFactor = 0.0f;
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

		VkStencilOpState stencilOpState = {};
		stencilOpState.failOp = VK_STENCIL_OP_KEEP;
		stencilOpState.passOp = VK_STENCIL_OP_KEEP;
		stencilOpState.depthFailOp = VK_STENCIL_OP_KEEP;
		stencilOpState.compareOp = VK_COMPARE_OP_ALWAYS;
		stencilOpState.compareMask = 0b00000000;
		stencilOpState.writeMask = 0b11111111;
		stencilOpState.reference = 0b00000000;

		m_depthStencilState = {};
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

		m_viewportState = {};
		m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_viewportState.viewportCount = 1;
		m_viewportState.scissorCount = 1;

		m_multisampleState = {};
		m_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_multisampleState.sampleShadingEnable = VK_FALSE;
		m_multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		m_multisampleState.minSampleShading = 0.0f;

		m_dynamicState = {};
		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.pDynamicStates = DYNAMIC_STATES.data();
		m_dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());
	}

	void Pipeline::CreateDescriptorPool()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes = {};
		descriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorPoolSizes[0].descriptorCount = 1;
		descriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorPoolSizes[1].descriptorCount = 1;

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
		descriptorPoolCreateInfo.maxSets = 50;

		Platform::ErrorVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
	}

	void Pipeline::CreateDescriptorSet()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts = std::vector<VkDescriptorSetLayout>();

		for (auto uniformBuffer : m_uniformBuffers)
		{
			descriptorSetLayouts.push_back(uniformBuffer->GetDescriptorSetLayout());
		}

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

		Platform::ErrorVk(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &m_descriptorSet));



		std::vector<VkWriteDescriptorSet> descriptorWrites = {};

		for (auto uniformBuffer : m_uniformBuffers)
		{
			VkDescriptorBufferInfo descriptorInfo = {};
			descriptorInfo.buffer = uniformBuffer->GetBuffer();
			descriptorInfo.offset = 0;
			descriptorInfo.range = uniformBuffer->GetSize();

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSet;
			descriptorWrite.dstBinding = 0; //  uniformBuffer->GetBinding();
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1; // descriptorSetLayouts.size();
			descriptorWrite.pBufferInfo = &descriptorInfo;

			descriptorWrites.push_back(descriptorWrite);
		}

		{
			VkDescriptorImageInfo descriptorInfo = {};
			descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			descriptorInfo.imageView = m_texture.GetImageView();
			descriptorInfo.sampler = m_texture.GetSampler();

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSet;
			descriptorWrite.dstBinding = 1;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &descriptorInfo;

			descriptorWrites.push_back(descriptorWrite);
		}

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void Pipeline::CreatePipelineLayout()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Gets all UBO descriptor sets.
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts = std::vector<VkDescriptorSetLayout>();

		for (auto uniformBuffer : m_uniformBuffers)
		{
			descriptorSetLayouts.push_back(uniformBuffer->GetDescriptorSetLayout());
		}

		// Pipeline layout struct.
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();

		// Creates the graphics pipeline layout.
		Platform::ErrorVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
	}

	void Pipeline::CreatePolygonPipeline()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto renderPass = Renderer::Get()->GetRenderPass();

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
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_shader->GetStages()->size());
		pipelineCreateInfo.pStages = m_shader->GetStages()->data();

		// Create the graphics pipeline.
		Platform::ErrorVk(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
	}

	void Pipeline::CreateNoDepthTestPipeline()
	{
	}

	void Pipeline::CreateMrtPipeline()
	{
	}

	void Pipeline::CreateMultiTexturePipeline()
	{
	}
}
