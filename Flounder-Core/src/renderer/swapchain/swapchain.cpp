#include "swapchain.h"

namespace flounder
{
	swapchain::swapchain()
	{
		m_swapChain = VK_NULL_HANDLE;
		m_swapChainImages = std::vector<VkImage>();
		m_swapChainImageViews = std::vector<VkImageView>();
		m_swapChainFramebuffers = std::vector<VkFramebuffer>();

		m_swapChainImageFormat = {};
		m_swapChainExtent = {};
	}

	swapchain::~swapchain()
	{
		cleanupFramebuffers();
		cleanup();
	}

	void swapchain::create(VkSwapChainSupportDetails swapChainSupport)
	{
		// Uses the helper functions to get optimal settings.
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		// Fills in the data from the create info.
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = display::get()->getVkSurface();

		// Gets the proper image count. 
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		// Attempts to create swap chain
		display::vkErrorCheck(vkCreateSwapchainKHR(display::get()->getVkDevice(), &createInfo, nullptr, &m_swapChain));

		// Populates the swap chain image vector.
		vkGetSwapchainImagesKHR(display::get()->getVkDevice(), m_swapChain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(display::get()->getVkDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

		// Stores the data for the chosen surface format and extent.
		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;

		createImageViews();
	}

	void swapchain::createImageViews()
	{
		m_swapChainImageViews.resize(m_swapChainImages.size());

		for (uint32_t i = 0; i < m_swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			display::vkErrorCheck(vkCreateImageView(display::get()->getVkDevice(), &createInfo, nullptr, &m_swapChainImageViews[i]));
		}

		std::cout << "Image views created successfully" << std::endl;
	}

	void swapchain::createFramebuffers(VkRenderPass renderPass)
	{
		m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

		for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
		{
			VkImageView attachments[] = {m_swapChainImageViews[i]};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_swapChainExtent.width;
			framebufferInfo.height = m_swapChainExtent.height;
			framebufferInfo.layers = 1;

			display::vkErrorCheck(vkCreateFramebuffer(display::get()->getVkDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]));
		}

		std::cout << "Framebuffers created successfully!" << std::endl;
	}

	void swapchain::cleanup()
	{
		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(display::get()->getVkDevice());

		for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
		{
			vkDestroyImageView(display::get()->getVkDevice(), m_swapChainImageViews[i], VK_NULL_HANDLE);
		}

		vkDestroySwapchainKHR(display::get()->getVkDevice(), m_swapChain, VK_NULL_HANDLE);
	}

	void swapchain::cleanupFramebuffers()
	{
		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(display::get()->getVkDevice());

		for (size_t i = 0; i < m_swapChainFramebuffers.size(); i++)
		{
			vkDestroyFramebuffer(display::get()->getVkDevice(), m_swapChainFramebuffers[i], VK_NULL_HANDLE);
		}
	}

	VkSurfaceFormatKHR swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
	{
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		}

		for (VkSurfaceFormatKHR availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
	{
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (VkPresentModeKHR availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	VkExtent2D swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = {};
			actualExtent.width = (uint32_t) display::get()->getWidth();
			actualExtent.height = (uint32_t) display::get()->getHeight();

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
}
