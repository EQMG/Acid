#include "Swapchain.hpp"
#include "../../devices/Display.hpp"

namespace Flounder
{
	Swapchain::Swapchain() :
		m_swapchain(VK_NULL_HANDLE),
		m_swapchainImages(std::vector<VkImage>()),
		m_swapchainImageViews(std::vector<VkImageView>()),
		m_swapchainFramebuffers(std::vector<VkFramebuffer>())
	{
	}

	Swapchain::~Swapchain()
	{
	}

	void Swapchain::Create(const VkDevice *logicalDevice, VkPhysicalDevice*physicalDevice, const VkSurfaceKHR *surface, GLFWwindow *window)
	{
		// Gets support details for the swap chain to pass to helper functions.
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(*physicalDevice, *surface);

		// Uses helper functions to retrieve optimal settings for swap chain.
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, window);

		// Initialize and create info struct for the swap chain.
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = *surface;

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

		// Attempts to create a swap chain.
		GlfwVulkan::ErrorCheck(vkCreateSwapchainKHR(*logicalDevice, &createInfo, nullptr, &m_swapchain));

		// Populates the swap chain image vector.
		vkGetSwapchainImagesKHR(*logicalDevice, m_swapchain, &imageCount, nullptr);
		m_swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(*logicalDevice, m_swapchain, &imageCount, m_swapchainImages.data());

		// Store data for the chosen surface format and extent.
		m_swapchainImageFormat = surfaceFormat.format;
		m_swapchainExtent = extent;

		CreateImageViews(logicalDevice);
	}

	void Swapchain::CreateFramebuffers(const VkDevice *logicalDevice, const VkRenderPass *renderPass)
	{
		m_swapchainFramebuffers.resize(m_swapchainImageViews.size());

		for (size_t i = 0; i < m_swapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = { m_swapchainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = *renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_swapchainExtent.width;
			framebufferInfo.height = m_swapchainExtent.height;
			framebufferInfo.layers = 1;

			GlfwVulkan::ErrorCheck(vkCreateFramebuffer(*logicalDevice, &framebufferInfo, nullptr, &m_swapchainFramebuffers[i]));
		}
	}

	void Swapchain::Cleanup(const VkDevice *logicalDevice)
	{
		for (size_t i = 0; i < m_swapchainImageViews.size(); i++)
		{
			vkDestroyImageView(*logicalDevice, m_swapchainImageViews[i], VK_NULL_HANDLE);
		}

		vkDestroySwapchainKHR(*logicalDevice, m_swapchain, VK_NULL_HANDLE);
	}

	void Swapchain::CleanupFrameBuffers(const VkDevice *logicalDevice)
	{
		for (size_t i = 0; i < m_swapchainFramebuffers.size(); i++)
		{
			vkDestroyFramebuffer(*logicalDevice, m_swapchainFramebuffers[i], VK_NULL_HANDLE);
		}
	}

	void Swapchain::CreateImageViews(const VkDevice* logicalDevice)
	{
		// Resizes the vector to the size of the images vector, define the deleter function for each.
		m_swapchainImageViews.resize(m_swapchainImages.size());

		// Iterates through each image and create an image view for each.
		for (uint32_t i = 0; i < m_swapchainImages.size(); i++)
		{
			// Defines a create info struct for this image view.
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapchainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapchainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			// Attempts to create the image view
			GlfwVulkan::ErrorCheck(vkCreateImageView(*logicalDevice, &createInfo, nullptr, &m_swapchainImageViews[i]));
		}
	}

	SwapChainSupportDetails Swapchain::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails details;

		// Sets capabilities variable on struct.
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
		
		// Sets formats vector on struct.
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		
		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		// Sets presentModes vector on struct.
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
		
		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR Swapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const
	{
		// Needs to set format and color space for VkSurfaceFormatKHR.

		// If the surface has no preferred format it returns a single VkSurfaceFormatKHR entry and format == VK_FORMAT_UNDEFINED.
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		}

		// If we cant choose any format, look through the list to see if the options we want are available.
		for (auto currentFormat : availableFormats)
		{
			if (currentFormat.format == VK_FORMAT_B8G8R8A8_UNORM && currentFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return currentFormat;
			}
		}

		// If both of these fail, just choose the first one available.
		return availableFormats[0];
	}

	VkPresentModeKHR Swapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const
	{
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (auto availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}

			if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	VkExtent2D Swapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) const
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		VkExtent2D actualExtent = {};
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, static_cast<uint32_t>(width)));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, static_cast<uint32_t>(height)));

		return actualExtent;
	}
}
