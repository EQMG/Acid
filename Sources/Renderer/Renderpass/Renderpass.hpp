#pragma once

#include "RenderpassCreate.hpp"

namespace acid
{
	class DepthStencil;

	/// <summary>
	/// A wrapper for <seealso cref="VkSubpassDescription"/> that has colour and depth stencil attachments stored by reference.
	/// </summary>
	class SubpassDescription :
		public VkSubpassDescription
	{
	public:
		std::vector<VkAttachmentReference> colorAttachments;
		VkAttachmentReference depthStencilAttachment;

		static VkSubpassDescription GetType(const SubpassDescription &subpassDescription)
		{
			auto type = static_cast<VkSubpassDescription>(subpassDescription);
			type.colorAttachmentCount = static_cast<uint32_t>(subpassDescription.colorAttachments.size());
			type.pColorAttachments = subpassDescription.colorAttachments.data();
			type.pDepthStencilAttachment = &subpassDescription.depthStencilAttachment;
			return type;
		}

		static std::vector<VkSubpassDescription> GetTypes(const std::vector<SubpassDescription> &subpassDescriptions)
		{
			std::vector<VkSubpassDescription> types = {};

			for (auto &subpassDescription : subpassDescriptions)
			{
				types.emplace_back(GetType(subpassDescription));
			}

			return types;
		}
	};

	class ACID_EXPORT Renderpass
	{
	private:
		VkRenderPass m_renderPass;
		std::vector<VkAttachmentDescription> m_attachments;
		std::vector<SubpassDescription> m_subpasses;
		std::vector<VkSubpassDependency> m_dependencies;
	public:
		Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Renderpass();

		VkRenderPass GetRenderpass() const { return m_renderPass; }
	};
}
