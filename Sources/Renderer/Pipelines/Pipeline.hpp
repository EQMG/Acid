#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Helpers/String.hpp"
#include "Files/Files.hpp"
#include "Renderer/Commands/CommandBuffer.hpp"
#include "ShaderProgram.hpp"

namespace acid
{
	/// <summary>
	/// A object that represents position in the renderpass/subpass structure.
	/// </summary>
	class ACID_EXPORT GraphicsStage
	{
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

		const uint32_t &GetRenderpass() const { return m_renderpass; }

		const uint32_t &GetSubpass() const { return m_subpass; }

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
	private:
		uint32_t m_renderpass;
		uint32_t m_subpass;
	};

	using ShaderDefine = std::pair<std::string, std::string>;

	class ACID_EXPORT VertexInput
	{
	public:
		VertexInput(const uint32_t &binding, const std::vector<VkVertexInputBindingDescription> &bindingDescriptions, const std::vector<VkVertexInputAttributeDescription> &attributeDescriptions) :
			m_binding(binding),
			m_bindingDescriptions(bindingDescriptions),
			m_attributeDescriptions(attributeDescriptions)
		{
		}

		const uint32_t &GetBinding() const { return m_binding; }

		const std::vector<VkVertexInputBindingDescription> &GetBindingDescriptions() const { return m_bindingDescriptions; }

		const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

		bool operator<(const VertexInput &other) const
		{
			return m_binding < other.m_binding;
		}
	private:
		uint32_t m_binding;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	};

	/// <summary>
	/// Class that represents is used to represent a Vulkan pipeline.
	/// </summary>
	class ACID_EXPORT Pipeline
	{
	protected:
		Pipeline() = default;

		~Pipeline() = default;
	public:
		void BindPipeline(const CommandBuffer &commandBuffer) const
		{
			vkCmdBindPipeline(commandBuffer.GetCommandBuffer(), GetPipelineBindPoint(), GetPipeline());
		}

		virtual const ShaderProgram *GetShaderProgram() const = 0;

		virtual const bool &IsPushDescriptors() const = 0;

		virtual const VkDescriptorSetLayout &GetDescriptorSetLayout() const = 0;

		virtual const VkDescriptorPool &GetDescriptorPool() const = 0;

		virtual const VkPipeline &GetPipeline() const = 0;

		virtual const VkPipelineLayout &GetPipelineLayout() const = 0;

		virtual const VkPipelineBindPoint &GetPipelineBindPoint() const = 0;
	};
}
