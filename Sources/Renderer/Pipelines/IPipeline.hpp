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

	using ShaderDefine = std::pair<std::string, std::string>;

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

	class ACID_EXPORT IPipeline
	{
	public:
		IPipeline() = default;

		~IPipeline() = default;

		void BindPipeline(const CommandBuffer &commandBuffer) const
		{
			vkCmdBindPipeline(commandBuffer.GetCommandBuffer(), GetPipelineBindPoint(), GetPipeline());
		}

		virtual ShaderProgram *GetShaderProgram() const = 0;

		virtual VkDescriptorSetLayout GetDescriptorSetLayout() const = 0;

		virtual VkDescriptorPool GetDescriptorPool() const = 0;

		virtual VkPipeline GetPipeline() const = 0;

		virtual VkPipelineLayout GetPipelineLayout() const = 0;

		virtual VkPipelineBindPoint GetPipelineBindPoint() const = 0;
	};
}
