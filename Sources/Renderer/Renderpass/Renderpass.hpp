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

		static VkSubpassDescription GetType(SubpassDescription &subpassDescription)
		{
			subpassDescription.colorAttachmentCount = static_cast<uint32_t>(subpassDescription.colorAttachments.size());
			subpassDescription.pColorAttachments = subpassDescription.colorAttachments.data();
			subpassDescription.pDepthStencilAttachment = &subpassDescription.depthStencilAttachment;
			return static_cast<VkSubpassDescription>(subpassDescription);
		}

		static std::vector<VkSubpassDescription> GetTypes(std::vector<SubpassDescription> &subpassDescriptions)
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

		const VkRenderPass &GetRenderpass() const { return m_renderPass; }
	};
}
