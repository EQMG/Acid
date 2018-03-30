#pragma once

#include <string>
#include <vector>
#include "../../Engine/Platform.hpp"

namespace Flounder
{
	enum PipelineModeFlags
	{
		PIPELINE_POLYGON,
		PIPELINE_POLYGON_NO_DEPTH,
		PIPELINE_MRT,
		PIPELINE_MRT_NO_DEPTH
	};

	struct GraphicsStage
	{
		unsigned int renderpass;
		uint32_t subpass;
	};

	class F_EXPORT DescriptorType
	{
	public:
		uint32_t m_binding;
		VkShaderStageFlags m_stage;

		VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
		VkDescriptorPoolSize m_descriptorPoolSize;

		DescriptorType(const uint32_t &binding, const VkShaderStageFlags &stage, const VkDescriptorSetLayoutBinding &descriptorSetLayoutBinding, const VkDescriptorPoolSize &descriptorPoolSize) :
			m_binding(binding),
			m_stage(stage),
			m_descriptorSetLayoutBinding(descriptorSetLayoutBinding),
			m_descriptorPoolSize(descriptorPoolSize)
		{
		}
	};

	class F_EXPORT PipelineCreate
	{
	public:
		std::vector<std::string> m_shaderStages;
		std::vector<VkVertexInputBindingDescription> m_vertexBindingDescriptions;

		PipelineModeFlags m_pipelineModeFlags;
		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullModeFlags;

		PipelineCreate(const std::vector<std::string> &shaderStages, const std::vector<VkVertexInputBindingDescription> &vertexBindingDescriptions,
					   const PipelineModeFlags &pipelineModeFlags = PIPELINE_POLYGON, const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullModeFlags = VK_CULL_MODE_BACK_BIT) :
			m_shaderStages(shaderStages),
			m_vertexBindingDescriptions(vertexBindingDescriptions),
			m_pipelineModeFlags(pipelineModeFlags),
			m_polygonMode(polygonMode),
			m_cullModeFlags(cullModeFlags)
		{
		}
	};
}
