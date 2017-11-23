#include "RenderPass.hpp"

#include <array>
#include "../../devices/Display.hpp"

namespace Flounder
{
	RenderPass::RenderPass(const VkFormat &depthFormat, const VkFormat &surfaceFormat) :
		m_renderPass(VK_NULL_HANDLE)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::array<VkAttachmentDescription, 6> attachments = {};

		for (uint32_t i = 0; i < 6; i++)
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
		attachments[2].format = VK_FORMAT_R8G8B8A8_UNORM; // Colour.
		attachments[3].format = VK_FORMAT_R8G8B8A8_UNORM; // Normal.
		attachments[4].format = VK_FORMAT_R8G8B8A8_UNORM; // Extras.
		attachments[5].format = VK_FORMAT_R16_SFLOAT; // Shadow.







		std::array<VkAttachmentReference, 1> subpass0ColourAttachments = {};
		subpass0ColourAttachments[0].attachment = 5; // Shadow.
		subpass0ColourAttachments[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference subpass1DepthStencilAttachment = {};
		subpass1DepthStencilAttachment.attachment = 0; // Depth.
		subpass1DepthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		std::array<VkAttachmentReference, 3> subpass1ColourAttachments = {};
		subpass1ColourAttachments[0].attachment = 2; // Colour.
		subpass1ColourAttachments[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		subpass1ColourAttachments[1].attachment = 3; // Normal.
		subpass1ColourAttachments[1].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		subpass1ColourAttachments[2].attachment = 4; // Extras.
		subpass1ColourAttachments[2].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		std::array<VkAttachmentReference, 1> subpass2ColourAttachments = {};
		subpass2ColourAttachments[0].attachment = 1; // Swapchain.
		subpass2ColourAttachments[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;







		std::array<VkSubpassDescription, 3> subpasses = {};
		subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[0].colorAttachmentCount = static_cast<uint32_t>(subpass0ColourAttachments.size());
		subpasses[0].pColorAttachments = subpass0ColourAttachments.data();

		subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[1].colorAttachmentCount = static_cast<uint32_t>(subpass1ColourAttachments.size());
		subpasses[1].pColorAttachments = subpass1ColourAttachments.data();
		subpasses[1].pDepthStencilAttachment = &subpass1DepthStencilAttachment;

		subpasses[2].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[2].colorAttachmentCount = static_cast<uint32_t>(subpass2ColourAttachments.size());
		subpasses[2].pColorAttachments = subpass2ColourAttachments.data();

		std::array<VkSubpassDependency, 2> dependencies = {};
		dependencies[0].srcSubpass = 0;
		dependencies[0].dstSubpass = 1;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		dependencies[1].srcSubpass = 1;
		dependencies[1].dstSubpass = 2;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

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

	RenderPass::~RenderPass()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyRenderPass(logicalDevice, m_renderPass, nullptr);
	}
}
