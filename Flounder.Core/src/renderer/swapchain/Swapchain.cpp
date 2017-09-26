#include "Swapchain.hpp"

namespace Flounder
{
	Swapchain::Swapchain() :
		m_presentMode(VK_PRESENT_MODE_FIFO_KHR),
		m_swapchain(VK_NULL_HANDLE),
		m_swapchainImageCount(0),
		m_swapchinImages(std::vector<VkImage>()),
		m_swapchinImageViews(std::vector<VkImageView>()),

		m_framebuffers(std::vector<VkFramebuffer>())
	{
	}

	Swapchain::~Swapchain()
	{
	}

	void Swapchain::Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkSurfaceCapabilitiesKHR &surfaceCapabilities, const VkSurfaceFormatKHR &surfaceFormat, const VkExtent2D &extent)
	{
		uint32_t physicalPresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount, nullptr);
		std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount,
			physicalPresentModes.data());

		for (auto presentMode : physicalPresentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				m_presentMode = presentMode;
			}
		}

		if (m_swapchainImageCount < surfaceCapabilities.minImageCount + 1)
		{
			m_swapchainImageCount = surfaceCapabilities.minImageCount + 1;
		}

		if (surfaceCapabilities.maxImageCount > 0)
		{
			if (m_swapchainImageCount > surfaceCapabilities.maxImageCount)
			{
				m_swapchainImageCount = surfaceCapabilities.maxImageCount;
			}
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = m_swapchainImageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = extent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = m_presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		GlfwVulkan::ErrorVk(vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &m_swapchain));

		GlfwVulkan::ErrorVk(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &m_swapchainImageCount, nullptr));

		m_swapchinImages.resize(m_swapchainImageCount);
		m_swapchinImageViews.resize(m_swapchainImageCount);

		GlfwVulkan::ErrorVk(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &m_swapchainImageCount, m_swapchinImages.data()));

		for (uint32_t i = 0; i < m_swapchainImageCount; i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = m_swapchinImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = surfaceFormat.format;
			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			GlfwVulkan::ErrorVk(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &m_swapchinImageViews[i]));
		}
	}

	void Swapchain::CreateFrameBuffers(const VkDevice &logicalDevice, const VkRenderPass &renderPass, const VkImageView &depthImageView, const VkExtent2D &extent)
	{
		m_framebuffers.resize(m_swapchainImageCount);

		for (uint32_t i = 0; i < m_swapchainImageCount; i++)
		{
			std::array<VkImageView, 2> attachments{};
			attachments[0] = depthImageView;
			attachments[1] = m_swapchinImageViews[i];

			VkFramebufferCreateInfo framebufferCreateInfo = {};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = renderPass;
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferCreateInfo.pAttachments = attachments.data();
			framebufferCreateInfo.width = extent.width;
			framebufferCreateInfo.height = extent.height;
			framebufferCreateInfo.layers = 1;

			GlfwVulkan::ErrorVk(vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
		}
	}

	void Swapchain::Cleanup(const VkDevice &logicalDevice)
	{
		for (auto imageView : m_swapchinImageViews)
		{
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice, m_swapchain, nullptr);
	}

	void Swapchain::CleanupFrameBuffers(const VkDevice &logicalDevice)
	{
		for (auto framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
		}
	}
}
