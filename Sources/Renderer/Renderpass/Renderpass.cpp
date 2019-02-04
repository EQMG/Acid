#include "Renderpass.hpp"

#include "Renderer/Renderer.hpp"
#include "Textures/DepthStencil.hpp"

namespace acid
{
	Renderpass::Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat, const VkSampleCountFlagBits &samples) :
		m_renderPass(VK_NULL_HANDLE),
		m_attachments(std::vector<VkAttachmentDescription>()),
		m_subpasses(std::vector<std::unique_ptr<SubpassDescription>>()),
		m_dependencies(std::vector<VkSubpassDependency>())
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		// Creates the renderpasses attachment descriptions,
		for (const auto &image : renderpassCreate.GetImages())
		{
			auto imageSamples = image.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;
			VkAttachmentDescription attachment = {};
			attachment.samples = imageSamples;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear at beginning of the render pass.
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // // The image can be read from so it's important to store the attachment results
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // We don't care about initial layout of the attachment.

			switch (image.GetType())
			{
			case Attachment::Type::Image:
				attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachment.format = image.GetFormat();
				break;
			case Attachment::Type::Depth:
				attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				attachment.format = depthStencil.GetFormat();
				break;
			case Attachment::Type::Swapchain:
				attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachment.format = surfaceFormat;
				break;
			}

			m_attachments.emplace_back(attachment);
		}

		// Creates each subpass and its dependencies.
		for (const auto &subpassType : renderpassCreate.GetSubpasses())
		{
			// Attachments.
			std::vector<VkAttachmentReference> subpassColourAttachments = {};

			std::optional<uint32_t> depthAttachment = {};

			for (const auto &attachmentBinding : subpassType.GetAttachmentBindings())
			{
				auto attachment = renderpassCreate.GetAttachment(attachmentBinding);

				if (!attachment)
				{
					Log::Error("Filed to find a renderpass attachment bound to: %i\n", attachmentBinding);
					continue;
				}

				if (renderpassCreate.GetImages().at(*attachment).GetType() == Attachment::Type::Depth)
				{
					depthAttachment = *attachment;
					continue;
				}

				VkAttachmentReference attachmentReference = {};
				attachmentReference.attachment = *attachment;
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				subpassColourAttachments.emplace_back(attachmentReference);
			}

			// Subpass description.
			m_subpasses.emplace_back(std::make_unique<SubpassDescription>(VK_PIPELINE_BIND_POINT_GRAPHICS, subpassColourAttachments, depthAttachment));

			// Subpass dependencies.
			VkSubpassDependency subpassDependency = {};
			subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			if (subpassType.GetBinding() == 0)
			{
				subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}
			else
			{
				subpassDependency.srcSubpass = subpassType.GetBinding() - 1;
			}

			if (subpassType.GetBinding() == renderpassCreate.GetSubpasses().size())
			{
				subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			}
			else
			{
				subpassDependency.dstSubpass = subpassType.GetBinding();
			}

			m_dependencies.emplace_back(subpassDependency);
		}

		std::vector<VkSubpassDescription> subpassDescriptions = {};

		for (const auto &subpass : m_subpasses)
		{
			subpassDescriptions.emplace_back(subpass->GetSubpassDescription());
		}

		// Creates the render pass.
		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
		renderPassCreateInfo.pAttachments = m_attachments.data();
		renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
		renderPassCreateInfo.pSubpasses = subpassDescriptions.data();
		renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(m_dependencies.size());
		renderPassCreateInfo.pDependencies = m_dependencies.data();
		Renderer::CheckVk(vkCreateRenderPass(logicalDevice->GetLogicalDevice(), &renderPassCreateInfo, nullptr, &m_renderPass));
	}

	Renderpass::~Renderpass()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		vkDestroyRenderPass(logicalDevice->GetLogicalDevice(), m_renderPass, nullptr);
	}
}
