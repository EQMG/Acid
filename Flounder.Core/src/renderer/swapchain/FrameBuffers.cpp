#include "FrameBuffers.hpp"

namespace Flounder
{
	FrameBuffers::FrameBuffers() :
		m_swapchainFramebuffers(std::vector<VkFramebuffer>())
	{
	}

	FrameBuffers::~FrameBuffers()
	{
	}

	void FrameBuffers::Create(const VkDevice *logicalDevice, const VkRenderPass &renderPass, const VkExtent2D &swapchainExtent, const std::vector<VkImageView> &swapchainImageViews)
	{

		m_swapchainFramebuffers.resize(swapchainImageViews.size());

		for (size_t i = 0; i < swapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = { swapchainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapchainExtent.width;
			framebufferInfo.height = swapchainExtent.height;
			framebufferInfo.layers = 1;

			GlfwVulkan::ErrorCheck(vkCreateFramebuffer(*logicalDevice, &framebufferInfo, nullptr, &m_swapchainFramebuffers[i]));
		}
	}

	void FrameBuffers::Cleanup(const VkDevice *logicalDevice)
	{
		for (size_t i = 0; i < m_swapchainFramebuffers.size(); i++)
		{
			vkDestroyFramebuffer(*logicalDevice, m_swapchainFramebuffers[i], VK_NULL_HANDLE);
		}
	}
}
