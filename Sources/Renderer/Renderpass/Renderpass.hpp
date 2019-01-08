#pragma once

#include <memory>
#include "Helpers/NonCopyable.hpp"
#include "RenderpassCreate.hpp"

namespace acid
{
	class DepthStencil;

	class SubpassDescription :
		public NonCopyable
	{
	private:
		VkSubpassDescription m_subpassDescription;
		std::unique_ptr<std::vector<VkAttachmentReference>> m_colorAttachments;
		std::unique_ptr<VkAttachmentReference> m_depthStencilAttachment;
	public:
		SubpassDescription(const VkSubpassDescription &subpassDescription, const std::vector<VkAttachmentReference> &colorAttachments, const std::optional<uint32_t> &depthAttachment) :
			m_subpassDescription(subpassDescription),
			m_colorAttachments(std::make_unique<std::vector<VkAttachmentReference>>(colorAttachments)),
			m_depthStencilAttachment(nullptr)
		{
			m_subpassDescription.colorAttachmentCount = static_cast<uint32_t>(m_colorAttachments->size());
			m_subpassDescription.pColorAttachments = m_colorAttachments->data();

			if (depthAttachment)
			{
				VkAttachmentReference depthStencilReference = {};
				depthStencilReference.attachment = *depthAttachment;
				depthStencilReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				m_depthStencilAttachment = std::make_unique<VkAttachmentReference>(depthStencilReference);
				m_subpassDescription.pDepthStencilAttachment = m_depthStencilAttachment.get();
			}
		}

		const VkSubpassDescription &GetSubpassDescription() const { return m_subpassDescription; }
	};

	class ACID_EXPORT Renderpass :
		public NonCopyable
	{
	private:
		VkRenderPass m_renderPass;
		std::vector<VkAttachmentDescription> m_attachments;
		std::vector<std::unique_ptr<SubpassDescription>> m_subpasses;
		std::vector<VkSubpassDependency> m_dependencies;
	public:
		Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Renderpass();

		const VkRenderPass &GetRenderpass() const { return m_renderPass; }
	};
}
