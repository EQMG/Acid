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

	struct GraphicsStage
	{
		unsigned int renderpass;
		uint32_t subpass;
	};

	struct PipelineCreate
	{
		PipelineModeFlags pipelineModeFlags = PIPELINE_POLYGON;
		VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
		VkCullModeFlags cullModeFlags = VK_CULL_MODE_BACK_BIT;

		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;

		std::vector<DescriptorType> descriptors = std::vector<DescriptorType>();

		std::vector<std::string> shaderStages = std::vector<std::string>();
	};
}
