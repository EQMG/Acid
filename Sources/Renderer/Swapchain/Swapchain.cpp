#include "Swapchain.hpp"

#include <array>
#include <cassert>
#include "Renderer/Renderer.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	Swapchain::Swapchain(const VkExtent2D &extent) :
		m_extent(extent),
		m_presentMode(VK_PRESENT_MODE_FIFO_KHR),
		m_imageCount(0),
		m_images(std::vector<VkImage>()),
		m_imageViews(std::vector<VkImageView>()),
		m_swapchain(VK_NULL_HANDLE),
		m_fenceImage(VK_NULL_HANDLE),
		m_activeImageIndex(std::numeric_limits<uint32_t>::max())
	{
		auto physicalDevice = Renderer::Get()->GetPhysicalDevice();
		auto surface = Renderer::Get()->GetSurface();
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		auto surfaceFormat = surface->GetFormat();
		auto surfaceCapabilities = surface->GetCapabilities();
		auto graphicsFamily = logicalDevice->GetGraphicsFamily();
		auto presentFamily = logicalDevice->GetPresentFamily();

		uint32_t physicalPresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->GetPhysicalDevice(), surface->GetSurface(), &physicalPresentModeCount, nullptr);
		std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->GetPhysicalDevice(), surface->GetSurface(), &physicalPresentModeCount, physicalPresentModes.data());

		for (const auto &presentMode : physicalPresentModes)
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

		m_imageCount = surfaceCapabilities.minImageCount + 1;

		if (surfaceCapabilities.maxImageCount > 0 && m_imageCount > surfaceCapabilities.maxImageCount)
		{
			m_imageCount = surfaceCapabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface->GetSurface();
		swapchainCreateInfo.minImageCount = m_imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = m_extent;
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
			swapchainCreateInfo.oldSwapchain = Renderer::Get()->GetSwapchain()->GetSwapchain();
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

		Window::CheckVk(vkCreateSwapchainKHR(logicalDevice->GetLogicalDevice(), &swapchainCreateInfo, nullptr, &m_swapchain));

		Window::CheckVk(vkGetSwapchainImagesKHR(logicalDevice->GetLogicalDevice(), m_swapchain, &m_imageCount, nullptr));
		m_images.resize(m_imageCount);
		m_imageViews.resize(m_imageCount);
		Window::CheckVk(vkGetSwapchainImagesKHR(logicalDevice->GetLogicalDevice(), m_swapchain, &m_imageCount, m_images.data()));

		for (uint32_t i = 0; i < m_imageCount; i++)
		{
			Texture::CreateImageView(m_images.at(i), m_imageViews.at(i), VK_IMAGE_VIEW_TYPE_2D, surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1);
		}

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vkCreateFence(logicalDevice->GetLogicalDevice(), &fenceCreateInfo, nullptr, &m_fenceImage);
	}

	Swapchain::~Swapchain()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		vkDestroyFence(logicalDevice->GetLogicalDevice(), m_fenceImage, nullptr);

		for (const auto &imageView : m_imageViews)
		{
			vkDestroyImageView(logicalDevice->GetLogicalDevice(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice->GetLogicalDevice(), m_swapchain, nullptr);
	}

	VkResult Swapchain::AcquireNextImage()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		VkResult acquireResult = vkAcquireNextImageKHR(logicalDevice->GetLogicalDevice(), m_swapchain, std::numeric_limits<uint64_t>::max(), VK_NULL_HANDLE, m_fenceImage, &m_activeImageIndex);

		if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
		{
			assert(false && "Renderer failed to acquire swapchain image!");
			return acquireResult;
		}

		Window::CheckVk(vkWaitForFences(logicalDevice->GetLogicalDevice(), 1, &m_fenceImage, VK_TRUE, std::numeric_limits<uint64_t>::max()));

		Window::CheckVk(vkResetFences(logicalDevice->GetLogicalDevice(), 1, &m_fenceImage));
		return VK_SUCCESS;
	}
}
