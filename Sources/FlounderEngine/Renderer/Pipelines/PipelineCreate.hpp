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

	class DescriptorType
	{
	public:
		uint32_t binding;
		VkShaderStageFlags stage;

		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;
		VkDescriptorPoolSize descriptorPoolSize;

		DescriptorType(const uint32_t &binding, const VkShaderStageFlags &stage, const VkDescriptorSetLayoutBinding &descriptorSetLayoutBinding, const VkDescriptorPoolSize &descriptorPoolSize) :
			binding(binding),
			stage(stage),
			descriptorSetLayoutBinding(descriptorSetLayoutBinding),
			descriptorPoolSize(descriptorPoolSize)
		{
		}
	};

	class PipelineCreateInfo
	{
	public:
		PipelineModeFlags pipelineModeFlags = PIPELINE_POLYGON;
		VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
		VkCullModeFlags cullModeFlags = VK_CULL_MODE_BACK_BIT;

		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;

		std::vector<DescriptorType> descriptors = std::vector<DescriptorType>();

		std::vector<std::string> shaderStages = std::vector<std::string>();
	};
}
