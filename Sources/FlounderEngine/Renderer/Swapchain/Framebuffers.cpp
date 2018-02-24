#include "Framebuffers.hpp"

#include <array>
#include "../../Devices/Display.hpp"

namespace Flounder
{
	Framebuffers::Framebuffers(const VkRenderPass &renderPass, const VkImageView &depthImageView, const Swapchain &swapchain, const VkExtent2D &extent) :
		m_framebuffers(std::vector<VkFramebuffer>())
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		m_framebuffers.resize(swapchain.GetImageCount());

		for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
		{
			std::vector<VkImageView> attachments(swapchain.GetImageAttachments().size() + 2);
			attachments[0] = depthImageView; // Depth
			attachments[1] = swapchain.GetImageViews()[i]; // Swapchain

			for (unsigned int j = 2; j < swapchain.GetImageAttachments().size(); j++)
			{
				attachments[j] = swapchain.GetTexture(j)->GetImageView();
			}

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
