#include "Renderpass.hpp"

#include "Display/Display.hpp"
#include "Renderer/Swapchain/DepthStencil.hpp"

namespace acid
{
	Renderpass::Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat) :
		m_renderPass(VK_NULL_HANDLE)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Attachments,
		std::vector<VkAttachmentDescription> attachments = {};

		for (auto &image : renderpassCreate.GetImages())
		{
			VkAttachmentDescription attachment = {};
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		//	attachment.samples = Display::Get()->GetMsaaSamples();
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			switch (image.GetType())
			{
			case ATTACHMENT_IMAGE:
				attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				attachment.format = static_cast<VkFormat>(image.GetFormat());
				break;
			case ATTACHMENT_DEPTH:
				attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				attachment.format = depthStencil.GetFormat();
				break;
			case ATTACHMENT_SWAPCHAIN:
				attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachment.format = surfaceFormat;
				break;
			}

			attachments.emplace_back(attachment);
		}

		// Subpasses and dependencies.
		std::vector<VkSubpassDescription> subpasses = {};
		std::vector<VkSubpassDependency> dependencies = {};

		for (auto &subpassType : renderpassCreate.GetSubpasses())
		{
			// Attachments.
			auto subpassColourAttachments = new std::vector<VkAttachmentReference>{}; // TODO: Not allocate memory like this.
			uint32_t depthAttachment = 9999;

			for (auto &attachment : subpassType.GetAttachments())
			{
				if (renderpassCreate.GetImages().at(attachment).GetType() == ATTACHMENT_DEPTH)
				{
					depthAttachment = attachment;
					continue;
				}

				VkAttachmentReference attachmentReference = {};
				attachmentReference.attachment = attachment;
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
				VkAttachmentReference subpassDepthStencilAttachment = {};
				subpassDepthStencilAttachment.attachment = depthAttachment;
				subpassDepthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				subpassDescription.pDepthStencilAttachment = &subpassDepthStencilAttachment;
			}

			subpasses.emplace_back(subpassDescription);

			// Dependencies.
			VkSubpassDependency subpassDependency = {};
			subpassDependency.srcAccessMask = 0;
			subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT; // VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT

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

			dependencies.emplace_back(subpassDependency);
		}

		// Creates the render pass.
		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassCreateInfo.pAttachments = attachments.data();
		renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
		renderPassCreateInfo.pSubpasses = subpasses.data();
		renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassCreateInfo.pDependencies = dependencies.data();

		Display::CheckVk(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &m_renderPass));
	}

	Renderpass::~Renderpass()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyRenderPass(logicalDevice, m_renderPass, nullptr);
	}
}
