#pragma once

#include <string>
#include <vector>
#include "Display/Display.hpp"

namespace fl
{
	enum PipelineMode
	{
		PIPELINE_MODE_POLYGON = 0,
		PIPELINE_MODE_POLYGON_NO_DEPTH = 1,
		PIPELINE_MODE_MRT = 2,
		PIPELINE_MODE_MRT_NO_DEPTH = 3
	};

	enum PipelinePolygonMode
	{
		PIPELINE_POLYGON_MODE_FILL = 0,
		PIPELINE_POLYGON_MODE_LINE = 1,
		PIPELINE_POLYGON_MODE_POINT = 2,
	};

	enum PipelineCullMode
	{
		PIPELINE_CULL_MODE_NONE = 0,
		PIPELINE_CULL_MODE_FRONT = 1,
		PIPELINE_CULL_MODE_BACK = 2,
		PIPELINE_CULL_MODE_ALL = 3,
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
	private:
		std::vector<std::string> m_shaderStages;
		VertexInput m_vertexInput;

		PipelineMode m_pipelineMode;
		PipelinePolygonMode m_polygonMode;
		PipelineCullMode m_cullMode;
	public:

		PipelineCreate(const std::vector<std::string> &shaderStages, const VertexInput &vertexInput,
					   const PipelineMode &pipelineMode = PIPELINE_MODE_POLYGON, const PipelinePolygonMode &polygonMode = PipelinePolygonMode::PIPELINE_POLYGON_MODE_FILL, const PipelineCullMode &cullMode = PipelineCullMode::PIPELINE_CULL_MODE_BACK) :
			m_shaderStages(shaderStages),
			m_vertexInput(vertexInput),
			m_pipelineMode(pipelineMode),
			m_polygonMode(polygonMode),
			m_cullMode(cullMode)
		{
		}

		std::vector<std::string> GetShaderStages() const { return m_shaderStages; }

		VertexInput GetVertexInput() const { return m_vertexInput; }

		PipelineMode GetMode() const { return m_pipelineMode; }

		PipelinePolygonMode GetPolygonMode() const { return m_polygonMode; }

		PipelineCullMode GetCullModeF() const { return m_cullMode; }
	};
}
