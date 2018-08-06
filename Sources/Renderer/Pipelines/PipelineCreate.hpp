#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Files/Files.hpp"

namespace acid
{
	enum PipelineMode
	{
		PIPELINE_MODE_POLYGON = 0,
		PIPELINE_MODE_POLYGON_NO_DEPTH = 1,
		PIPELINE_MODE_MRT = 2,
		PIPELINE_MODE_MRT_NO_DEPTH = 3,
		PIPELINE_MODE_COMPUTE = 4
	};

	class ACID_EXPORT GraphicsStage
	{
	private:
		uint32_t m_renderpass;
		uint32_t m_subpass;
	public:
		GraphicsStage(const uint32_t &renderpass, const uint32_t &subpass) :
			m_renderpass(renderpass),
			m_subpass(subpass)
		{
		}

		uint32_t GetRenderpass() const { return m_renderpass; }

		uint32_t GetSubpass() const { return m_subpass; }
	};

	class ACID_EXPORT VertexInput
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

	class ACID_EXPORT DescriptorType
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

	class ACID_EXPORT PipelineDefine
	{
	private:
		std::string m_name;
		std::string m_value;
	public:
		PipelineDefine(const std::string &name, const std::string &value) :
			m_name(name),
			m_value(value)
		{
		}

		std::string GetName() const { return m_name; }

		std::string GetValue() const { return m_value; }
	};

	class ACID_EXPORT PipelineCreate
	{
	private:
		std::vector<std::string> m_shaderStages;
		VertexInput m_vertexInput;

		PipelineMode m_pipelineMode;

		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullMode;

		std::vector<PipelineDefine> m_defines;
	public:

		PipelineCreate(const std::vector<std::string> &shaderStages, const VertexInput &vertexInput, const PipelineMode &pipelineMode = PIPELINE_MODE_POLYGON,
						const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const std::vector<PipelineDefine> &defines = {}) :
			m_shaderStages(shaderStages),
			m_vertexInput(vertexInput),
			m_pipelineMode(pipelineMode),
			m_polygonMode(polygonMode),
			m_cullMode(cullMode),
			m_defines(defines)
		{
			for (auto &shaderStage : m_shaderStages)
			{
				shaderStage = Files::SearchFile(shaderStage);
			}
		}

		std::vector<std::string> GetShaderStages() const { return m_shaderStages; }

		VertexInput GetVertexInput() const { return m_vertexInput; }

		PipelineMode GetMode() const { return m_pipelineMode; }

		VkPolygonMode GetPolygonMode() const { return m_polygonMode; }

		VkCullModeFlags GetCullMode() const { return m_cullMode; }

		std::vector<PipelineDefine> GetDefines() const { return m_defines; }
	};

	class ACID_EXPORT ComputeCreate
	{
	private:
		std::string m_shaderStage;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_workgroupSize;

		std::vector<PipelineDefine> m_defines;
	public:

		ComputeCreate(const std::string &shaderStage, const uint32_t &width, const uint32_t &height, const uint32_t &workgroupSize,
						const std::vector<PipelineDefine> &defines = {}) :
			m_shaderStage(Files::SearchFile(shaderStage)),
			m_width(width),
			m_height(height),
			m_workgroupSize(workgroupSize),
			m_defines(defines)
		{
			m_defines.emplace_back(PipelineDefine("WIDTH", std::to_string(m_width)));
			m_defines.emplace_back(PipelineDefine("HEIGHT", std::to_string(m_height)));
			m_defines.emplace_back(PipelineDefine("WORKGROUP_SIZE", std::to_string(m_workgroupSize)));
		}

		std::string GetShaderStage() const { return m_shaderStage; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		uint32_t GetWorkgroupSize() const { return m_workgroupSize; }

		std::vector<PipelineDefine> GetDefines() const { return m_defines; }
	};
}
