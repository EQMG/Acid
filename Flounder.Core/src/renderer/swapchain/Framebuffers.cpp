#include "Framebuffers.hpp"

#include <array>
#include "../../devices/Display.hpp"

namespace Flounder
{
	Framebuffers::Framebuffers(const VkRenderPass &renderPass, const VkImageView &depthImageView, const Swapchain &swapchain, const VkExtent2D &extent) :
		m_framebuffers(std::vector<VkFramebuffer>())
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		m_framebuffers.resize(swapchain.GetImageCount());

		for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
		{
			std::array<VkImageView, 6> attachments = {};
			attachments[0] = depthImageView; // Depth.
			attachments[1] = swapchain.GetImageViews()[i]; // Swapchain.
			attachments[2] = swapchain.GetColourImage()->GetImageView(); // Colours.
			attachments[3] = swapchain.GetNormalImage()->GetImageView(); // Normals.
			attachments[4] = swapchain.GetExtrasImage()->GetImageView(); // Extras.
			attachments[5] = swapchain.GetShadowImage()->GetImageView(); // Shadows.

			VkFramebufferCreateInfo framebufferCreateInfo = {};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = renderPass;
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferCreateInfo.pAttachments = attachments.data();
			framebufferCreateInfo.width = extent.width;
			framebufferCreateInfo.height = extent.height;
			framebufferCreateInfo.layers = 1;

			Platform::ErrorVk(vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
		}
	}

	Framebuffers::~Framebuffers()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
		}
	}
}
