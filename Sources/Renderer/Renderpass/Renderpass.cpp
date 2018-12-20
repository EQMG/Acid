#include "Renderpass.hpp"

#include "Display/Display.hpp"
#include "Renderer/Swapchain/DepthStencil.hpp"

namespace acid
{
	Renderpass::Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat, const VkSampleCountFlagBits &samples) :
		m_renderPass(VK_NULL_HANDLE),
		m_attachments(std::vector<VkAttachmentDescription>()),
		m_subpasses(std::vector<VkSubpassDescription>()),
		m_dependencies(std::vector<VkSubpassDependency>())
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Attachments,
		for (auto &image : renderpassCreate.GetImages())
		{
			auto imageSamples = image.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;
			VkAttachmentDescription attachment = {};
			attachment.samples = imageSamples;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			switch (image.GetType())
			{
			case ATTACHMENT_TYPE_IMAGE:
				attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachment.format = image.GetFormat();
				break;
			case ATTACHMENT_TYPE_DEPTH:
				attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				attachment.format = depthStencil.GetFormat();
				break;
			case ATTACHMENT_TYPE_SWAPCHAIN:
				attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachment.format = surfaceFormat;
				break;
			}

			m_attachments.emplace_back(attachment);
		}

		// Subpasses and dependencies.
		for (auto &subpassType : renderpassCreate.GetSubpasses())
		{
			// Attachments.
			auto subpassColourAttachments = new std::vector<VkAttachmentReference>(); // FIXME: Fixes a SEGFAULT on Linux.

			uint32_t depthAttachment = 9999;

			for (auto &attachmentBinding : subpassType.GetAttachmentBindings())
			{
				auto attachment = renderpassCreate.GetAttachment(attachmentBinding);

				if (!attachment)
				{
					Log::Error("Filed to find a renderpass attachment bound to: %i\n", attachmentBinding);
					continue;
				}

				if (renderpassCreate.GetImages().at(*attachment).GetType() == ATTACHMENT_TYPE_DEPTH)
				{
					depthAttachment = *attachment;
					continue;
				}

				VkAttachmentReference attachmentReference = {};
				attachmentReference.attachment = *attachment;
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				subpassColourAttachments->emplace_back(attachmentReference);
			}

			// Description.
			VkSubpassDescription subpassDescription = {};
			subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassDescription.colorAttachmentCount = static_cast<uint32_t>(subpassColourAttachments->size());
			subpassDescription.pColorAttachments = subpassColourAttachments->data();

			if (depthAttachment != 9999)
			{
				auto subpassDepthStencilReference = new VkAttachmentReference(); // FIXME: Fixes a SEGFAULT on Linux.
				subpassDepthStencilReference->attachment = depthAttachment;
				subpassDepthStencilReference->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				subpassDescription.pDepthStencilAttachment = subpassDepthStencilReference;
			}

			m_subpasses.emplace_back(subpassDescription);

			// Dependencies.
			VkSubpassDependency subpassDependency = {};
			subpassDependency.srcAccessMask = 0;
			subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT; // VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			if (renderpassCreate.GetSubpasses().size() == 1)
			{
				subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			}
			else if (subpassType.GetBinding() == 0)
			{
				subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			}
			else
			{
				subpassDependency.srcSubpass = subpassType.GetBinding() - 1;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			}

			if (subpassType.GetBinding() == renderpassCreate.GetSubpasses().size())
			{
				subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			}
			else
			{
				subpassDependency.dstSubpass = subpassType.GetBinding();
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}

			m_dependencies.emplace_back(subpassDependency);
		}

		// Creates the render pass.
		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
		renderPassCreateInfo.pAttachments = m_attachments.data();
		renderPassCreateInfo.subpassCount = static_cast<uint32_t>(m_subpasses.size());
		renderPassCreateInfo.pSubpasses = m_subpasses.data();
		renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(m_dependencies.size());
		renderPassCreateInfo.pDependencies = m_dependencies.data();
		Display::CheckVk(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &m_renderPass));
	}

	Renderpass::~Renderpass()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyRenderPass(logicalDevice, m_renderPass, nullptr);
	}
}
