﻿#include "RenderPass.hpp"

namespace Flounder
{
	RenderPass::RenderPass() :
		m_renderPass(VK_NULL_HANDLE)
	{
	}

	RenderPass::~RenderPass()
	{
	}

	void RenderPass::Create(const VkDevice &logicalDevice, const VkFormat &depthFormat, const VkFormat &surfaceFormat)
	{
		std::array<VkAttachmentDescription, 2> attachments = {};
		attachments[0].flags = 0;
		attachments[0].format = depthFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		attachments[1].flags = 0;
		attachments[1].format = surfaceFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		std::array<VkAttachmentReference, 1> subpass0ColourAttachments = {};
		subpass0ColourAttachments[0].attachment = 1;
		subpass0ColourAttachments[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference subpass0DepthStencilAttachment = {};
		subpass0DepthStencilAttachment.attachment = 0;
		subpass0DepthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

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

		GlfwVulkan::ErrorVk(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &m_renderPass));
	}

	void RenderPass::Cleanup(const VkDevice &logicalDevice)
	{
		vkDestroyRenderPass(logicalDevice, m_renderPass, nullptr);
	}
}
