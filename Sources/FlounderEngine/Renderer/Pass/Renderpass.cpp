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
		std::vector<VkAttachmentDescription> attachments(renderpassCreate.images.size() + 2);

		for (uint32_t i = 0; i < attachments.size(); i++)
		{
			attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
			attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachments[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachments[i].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}

		attachments[0].format = depthFormat; // Depth.
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachments[1].format = surfaceFormat; // Swapchain.
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		for (unsigned int j = 0; j < renderpassCreate.images.size(); j++)
		{
			attachments[j + 2].format = renderpassCreate.images.at(j).m_format;
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
