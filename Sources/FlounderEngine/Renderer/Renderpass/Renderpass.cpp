#include "Renderpass.hpp"

#include <array>
#include "../../Devices/Display.hpp"

namespace Flounder
{
	Renderpass::Renderpass(const RenderpassCreate &renderpassCreate, const VkFormat &depthFormat, const VkFormat &surfaceFormat) :
		m_renderPass(VK_NULL_HANDLE)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Attachments,
		std::vector<VkAttachmentDescription> attachments = {};

		for (auto image : renderpassCreate.images)
		{
			VkAttachmentDescription attachment = {};
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

			switch (image.m_type)
			{
			case TypeImage:
				attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				attachment.format = image.m_format;
				break;
			case TypeDepth:
				attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				attachment.format = depthFormat;
				break;
			case TypeSwapchain:
				attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachment.format = surfaceFormat;
				break;
			}

			attachments.push_back(attachment);
		}

		// Subpasses and dependencies.
		std::vector<VkSubpassDescription> subpasses = {};
		std::vector<VkSubpassDependency> dependencies = {};

		for (auto subpassType : renderpassCreate.subpasses)
		{
			// Description.
			std::vector<VkAttachmentReference> subpassColourAttachments = {};

			for (unsigned int attachment : subpassType.m_attachments)
			{
				VkAttachmentReference attachmentReference = {};
				attachmentReference.attachment = attachment;
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				subpassColourAttachments.push_back(attachmentReference);
			}

			VkSubpassDescription subpassDescription = {};
			subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassDescription.colorAttachmentCount = static_cast<uint32_t>(subpassColourAttachments.size());
			subpassDescription.pColorAttachments = subpassColourAttachments.data();

			if (subpassType.m_useDepth)
			{
				VkAttachmentReference subpassDepthStencilAttachment = {};
				subpassDepthStencilAttachment.attachment = 0;
				subpassDepthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				subpassDescription.pDepthStencilAttachment = &subpassDepthStencilAttachment;
			}

			subpasses.push_back(subpassDescription);

			// Dependencies.
			VkSubpassDependency subpassDependency = {};

			if (subpassType.m_binding == 0)
			{
				subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			}
			else
			{
				subpassDependency.srcSubpass = subpassType.m_binding - 1;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			}

			if (subpassType.m_binding == renderpassCreate.subpasses.size())
			{
				subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			}
			else
			{
				subpassDependency.dstSubpass = subpassType.m_binding;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}

			subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			dependencies.push_back(subpassDependency);
		}


		if (renderpassCreate.subpasses.size() != 1)
		{
			dependencies.clear();
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

		Platform::ErrorVk(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &m_renderPass));
	}

	Renderpass::~Renderpass()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyRenderPass(logicalDevice, m_renderPass, nullptr);
	}
}
