#include "Swapchain.hpp"

#include "Display/Display.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	Swapchain::Swapchain(const VkExtent2D &extent) :
		m_presentMode(VK_PRESENT_MODE_FIFO_KHR),
		m_swapchain(VK_NULL_HANDLE),
		m_swapchainImageCount(0),
		m_swapchainImages(std::vector<VkImage>()),
		m_swapchainImageViews(std::vector<VkImageView>()),
		m_extent({})
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto physicalDevice = Display::Get()->GetPhysicalDevice();
		auto surface = Display::Get()->GetSurface();
		auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		auto surfaceCapabilities = Display::Get()->GetSurfaceCapabilities();
		auto graphicsFamily = Display::Get()->GetGraphicsFamily();
		auto presentFamily = Display::Get()->GetPresentFamily();

		m_extent = extent;

		uint32_t physicalPresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount, nullptr);
		std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount, physicalPresentModes.data());

		for (auto &presentMode : physicalPresentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				m_presentMode = presentMode;
				break;
			}
			else if (presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				m_presentMode = presentMode;
			}
		}

		m_swapchainImageCount = surfaceCapabilities.minImageCount + 1;

		if (surfaceCapabilities.maxImageCount > 0 && m_swapchainImageCount > surfaceCapabilities.maxImageCount)
		{
			m_swapchainImageCount = surfaceCapabilities.maxImageCount;
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
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = m_presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
		{
			swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		{
			swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		if (Renderer::Get()->GetSwapchain() != nullptr)
		{
			swapchainCreateInfo.oldSwapchain = *Renderer::Get()->GetSwapchain()->GetSwapchain();
		}

		if (graphicsFamily != presentFamily)
		{
			std::array<uint32_t, 2> queueFamily = { graphicsFamily, presentFamily };
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
			swapchainCreateInfo.pQueueFamilyIndices = queueFamily.data();
		}
		else
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		Display::CheckVk(vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &m_swapchain));

		Display::CheckVk(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &m_swapchainImageCount, nullptr));
		m_swapchainImages.resize(m_swapchainImageCount);
		m_swapchainImageViews.resize(m_swapchainImageCount);
		Display::CheckVk(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &m_swapchainImageCount, m_swapchainImages.data()));

		for (uint32_t i = 0; i < m_swapchainImageCount; i++)
		{
			Texture::CreateImageView(m_swapchainImages.at(i), m_swapchainImageViews.at(i), VK_IMAGE_VIEW_TYPE_2D, surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1);
		}
	}

	Swapchain::~Swapchain()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto &imageView : m_swapchainImageViews)
		{
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice, m_swapchain, nullptr);
	}
}
