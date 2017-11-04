#include "RenderPass.hpp"

#include <array>
#include "../../devices/Display.hpp"
#include "../RenderDeferred.hpp"

namespace Flounder
{
	RenderPass::RenderPass(const VkFormat &depthFormat, const VkFormat &surfaceFormat) :
		m_renderPass(VK_NULL_HANDLE)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::array<VkAttachmentDescription, DeferredCount> attachments = {};

		attachments[DeferredDepth].flags = 0;
		attachments[DeferredDepth].format = depthFormat;
		attachments[DeferredDepth].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[DeferredDepth].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[DeferredDepth].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[DeferredDepth].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[DeferredDepth].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[DeferredDepth].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[DeferredDepth].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		attachments[DeferredColour].flags = 0;
		attachments[DeferredColour].format = surfaceFormat;
		attachments[DeferredColour].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[DeferredColour].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[DeferredColour].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[DeferredColour].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[DeferredColour].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		//attachments[DeferredNormal].flags = 0;
		//attachments[DeferredNormal].format = surfaceFormat;
		//attachments[DeferredNormal].samples = VK_SAMPLE_COUNT_1_BIT;
		//attachments[DeferredNormal].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		//attachments[DeferredNormal].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		//attachments[DeferredNormal].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		//attachments[DeferredNormal].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkAttachmentReference subpass0DepthStencilAttachment = {};
		subpass0DepthStencilAttachment.attachment = 0;
		subpass0DepthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		std::array<VkAttachmentReference, DeferredCount - 1> subpass0ColourAttachments = {};

		for (uint32_t i = 0; i < DeferredCount - 1; i++)
		{
			subpass0ColourAttachments[i].attachment = i + 1;
			subpass0ColourAttachments[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		std::array<VkSubpassDescription, 1> subpasses = {};
		subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[0].colorAttachmentCount = static_cast<uint32_t>(subpass0ColourAttachments.size());
		subpasses[0].pColorAttachments = subpass0ColourAttachments.data();
		subpasses[0].pDepthStencilAttachment = &subpass0DepthStencilAttachment;

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassCreateInfo.pAttachments = attachments.data();
		renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
		renderPassCreateInfo.pSubpasses = subpasses.data();

		Platform::ErrorVk(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &m_renderPass));
	}

	RenderPass::~RenderPass()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyRenderPass(logicalDevice, m_renderPass, nullptr);
	}
}
