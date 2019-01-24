#pragma once

#include <memory>
#include "Helpers/NonCopyable.hpp"
#include "RenderpassCreate.hpp"

namespace acid
{
	class DepthStencil;

	class ACID_EXPORT Renderpass :
		public NonCopyable
	{
	public:
		class SubpassDescription :
			public NonCopyable
		{
		public:
			SubpassDescription(const VkPipelineBindPoint &bindPoint, const std::vector<VkAttachmentReference> &colorAttachments, const std::optional<uint32_t> &depthAttachment) :
				m_subpassDescription({}),
				m_colorAttachments(colorAttachments),
				m_depthStencilAttachment({})
			{
				m_subpassDescription.pipelineBindPoint = bindPoint;
				m_subpassDescription.colorAttachmentCount = static_cast<uint32_t>(m_colorAttachments.size());
				m_subpassDescription.pColorAttachments = m_colorAttachments.data();

				if (depthAttachment)
				{
					m_depthStencilAttachment.attachment = *depthAttachment;
					m_depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					m_subpassDescription.pDepthStencilAttachment = &m_depthStencilAttachment;
				}
			}

			const VkSubpassDescription &GetSubpassDescription() const { return m_subpassDescription; }
		private:
			VkSubpassDescription m_subpassDescription;
			std::vector<VkAttachmentReference> m_colorAttachments;
			VkAttachmentReference m_depthStencilAttachment;
		};

		Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Renderpass();

		const VkRenderPass &GetRenderpass() const { return m_renderPass; }
	private:
		VkRenderPass m_renderPass;
		std::vector<VkAttachmentDescription> m_attachments;
		std::vector<std::unique_ptr<SubpassDescription>> m_subpasses;
		std::vector<VkSubpassDependency> m_dependencies;
	};
}
