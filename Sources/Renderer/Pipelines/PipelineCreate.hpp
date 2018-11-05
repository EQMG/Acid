#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Helpers/String.hpp"
#include "Files/Files.hpp"

namespace acid
{
	enum PipelineMode
	{
		PIPELINE_MODE_POLYGON = 0,
		PIPELINE_MODE_MRT = 1,
		PIPELINE_MODE_COMPUTE = 2
	};

	enum PipelineDepth
	{
		PIPELINE_DEPTH_NONE = 0,
		PIPELINE_DEPTH_READ_WRITE = 1,
		PIPELINE_DEPTH_READ = 2,
		PIPELINE_DEPTH_WRITE = 3
	};

	/// <summary>
	/// A object that represents position in the renderpass/subpass structure.
	/// </summary>
	class ACID_EXPORT GraphicsStage
	{
	private:
		uint32_t m_renderpass;
		uint32_t m_subpass;
	public:
		/// <summary>
		/// Creates a new graphics stage.
		/// </summary>
		/// <param name="renderpass"> The renderpass. </param>
		/// <param name="subpass"> The subpass. </param>
		GraphicsStage(const uint32_t &renderpass, const uint32_t &subpass) :
			m_renderpass(renderpass),
			m_subpass(subpass)
		{
		}

		uint32_t GetRenderpass() const { return m_renderpass; }

		uint32_t GetSubpass() const { return m_subpass; }

		bool operator==(const GraphicsStage &other) const
		{
			return m_renderpass == other.m_renderpass && m_subpass == other.m_subpass;
		}

		bool operator!=(const GraphicsStage &other) const
		{
			return !(*this == other);
		}

		bool operator<(const GraphicsStage &other) const
		{
			return m_renderpass < other.m_renderpass || m_subpass < other.m_subpass;
		}
	};

	class ACID_EXPORT VertexInput
	{
	private:
		uint32_t m_binding;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	public:
		VertexInput(const uint32_t &binding, const std::vector<VkVertexInputBindingDescription> &bindingDescriptions, const std::vector<VkVertexInputAttributeDescription> &attributeDescriptions) :
			m_binding(binding),
			m_bindingDescriptions(bindingDescriptions),
			m_attributeDescriptions(attributeDescriptions)
		{
		}

		uint32_t GetBinding() const { return m_binding; }

		std::vector<VkVertexInputBindingDescription> GetBindingDescriptions() const { return m_bindingDescriptions; }

		std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() const { return m_attributeDescriptions; }

		bool operator<(const VertexInput &other) const
		{
			return m_binding < other.m_binding;
		}
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
		std::vector<VertexInput> m_vertexInputs;

		PipelineMode m_pipelineMode;
		PipelineDepth m_depthMode;
		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullMode;

		std::vector<PipelineDefine> m_defines;
	public:
		PipelineCreate(const std::vector<std::string> &shaderStages, const std::vector<VertexInput> &vertexInputs, const PipelineMode &pipelineMode = PIPELINE_MODE_POLYGON, const PipelineDepth &depthMode = PIPELINE_DEPTH_READ_WRITE,
						const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const std::vector<PipelineDefine> &defines = {}) :
			m_shaderStages(shaderStages),
			m_vertexInputs(vertexInputs),
			m_pipelineMode(pipelineMode),
			m_depthMode(depthMode),
			m_polygonMode(polygonMode),
			m_cullMode(cullMode),
			m_defines(defines)
		{
			std::sort(m_vertexInputs.begin(), m_vertexInputs.end());
		}

		std::vector<std::string> GetShaderStages() const { return m_shaderStages; }

		std::vector<VertexInput> GetVertexInputs() const { return m_vertexInputs; }

		PipelineMode GetPipelineMode() const { return m_pipelineMode; }

		PipelineDepth GetPipelineDepth() const { return m_depthMode; }

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
			m_shaderStage(shaderStage),
			m_width(width),
			m_height(height),
			m_workgroupSize(workgroupSize),
			m_defines(defines)
		{
			m_defines.emplace_back(PipelineDefine("WIDTH", String::To(m_width)));
			m_defines.emplace_back(PipelineDefine("HEIGHT", String::To(m_height)));
			m_defines.emplace_back(PipelineDefine("WORKGROUP_SIZE", String::To(m_workgroupSize)));
		}

		std::string GetShaderStage() const { return m_shaderStage; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		uint32_t GetWorkgroupSize() const { return m_workgroupSize; }

		std::vector<PipelineDefine> GetDefines() const { return m_defines; }
	};
}
