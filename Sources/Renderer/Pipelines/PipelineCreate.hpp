#pragma once

#include <string>
#include <vector>
#include "Display/Display.hpp"

namespace fl
{
	enum PipelineModeFlags
	{
		PIPELINE_POLYGON = 0,
		PIPELINE_POLYGON_NO_DEPTH = 1,
		PIPELINE_MRT = 2,
		PIPELINE_MRT_NO_DEPTH = 3
	};

	class FL_EXPORT GraphicsStage
	{
	private:
		unsigned int m_renderpass;
		uint32_t m_subpass;
	public:
		GraphicsStage(const unsigned int &renderpass, const uint32_t &subpass) :
			m_renderpass(renderpass),
			m_subpass(subpass)
		{
		}

		unsigned int GetRenderpass() const { return m_renderpass; }

		uint32_t GetSubpass() const { return m_subpass; }
	};

	class FL_EXPORT VertexInput
	{
	private:
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	public:
		VertexInput(const std::vector<VkVertexInputBindingDescription> &bindingDescriptions, const std::vector<VkVertexInputAttributeDescription> &attributeDescriptions) :
			m_bindingDescriptions(bindingDescriptions),
			m_attributeDescriptions(attributeDescriptions)
		{
		}

		std::vector<VkVertexInputBindingDescription> GetBindingDescriptions() const { return m_bindingDescriptions; }

		std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() const { return m_attributeDescriptions; }
	};

	class FL_EXPORT DescriptorType
	{
	private:
		uint32_t m_binding;
		VkShaderStageFlags m_stage;

		VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
		VkDescriptorPoolSize m_descriptorPoolSize;
	public:
		DescriptorType(const uint32_t &binding, const VkShaderStageFlags &stage, const VkDescriptorSetLayoutBinding &descriptorSetLayoutBinding, const VkDescriptorPoolSize &descriptorPoolSize) :
			m_binding(binding),
			m_stage(stage),
			m_descriptorSetLayoutBinding(descriptorSetLayoutBinding),
			m_descriptorPoolSize(descriptorPoolSize)
		{
		}

		uint32_t GetBinding() const { return m_binding; }

		VkShaderStageFlags GetStage() const { return m_stage; }

		VkDescriptorSetLayoutBinding GetLayoutBinding() const { return m_descriptorSetLayoutBinding; }

		VkDescriptorPoolSize GetPoolSize() const { return m_descriptorPoolSize; }
	};

	class FL_EXPORT PipelineCreate
	{
	public:
		std::vector<std::string> m_shaderStages;
	private:
		VertexInput m_vertexInput;

		PipelineModeFlags m_pipelineModeFlags;
		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullModeFlags;
	public:

		PipelineCreate(const std::vector<std::string> &shaderStages, const VertexInput &vertexInput,
					   const PipelineModeFlags &pipelineModeFlags = PIPELINE_POLYGON, const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullModeFlags = VK_CULL_MODE_BACK_BIT) :
			m_shaderStages(shaderStages),
			m_vertexInput(vertexInput),
			m_pipelineModeFlags(pipelineModeFlags),
			m_polygonMode(polygonMode),
			m_cullModeFlags(cullModeFlags)
		{
		}

		VertexInput GetVertexInput() const { return m_vertexInput; }

		PipelineModeFlags GetModeFlags() const { return m_pipelineModeFlags; }

		VkPolygonMode GetPolygonMode() const { return m_polygonMode; }

		VkCullModeFlags GetCullModeFlags() const { return m_cullModeFlags; }
	};
}
