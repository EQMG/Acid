#include "Framebuffers.hpp"

#include <array>
#include "../../devices/Display.hpp"

namespace Flounder
{
	Framebuffers::Framebuffers(const VkRenderPass &renderPass, const VkImageView &depthImageView, const VkExtent2D &extent, const uint32_t &swapchainImageCount, const std::vector<VkImageView> &swapchinImageViews, const VkImageView &colourImageView, const VkImageView &normalImageView) :
		m_framebuffers(std::vector<VkFramebuffer>())
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		m_framebuffers.resize(swapchainImageCount);

		for (uint32_t i = 0; i < swapchainImageCount; i++)
		{
			std::array<VkImageView, 4> attachments = {};
			attachments[0] = depthImageView;
			attachments[1] = swapchinImageViews[i];
			attachments[2] = colourImageView; 
			attachments[3] = normalImageView;

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
