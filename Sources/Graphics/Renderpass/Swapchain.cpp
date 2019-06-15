#include "Swapchain.hpp"

#include "Graphics/Graphics.hpp"

namespace acid
{
static const std::vector<VkCompositeAlphaFlagBitsKHR> COMPOSITE_ALPHA_FLAGS{ VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
	VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR, VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR, };

Swapchain::Swapchain(const VkExtent2D &extent, const std::optional<Reference<Swapchain>> &oldSwapchain) :
	m_extent{extent},
	m_presentMode{VK_PRESENT_MODE_FIFO_KHR},
	m_preTransform{VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR},
	m_compositeAlpha{VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR},
	m_activeImageIndex{std::numeric_limits<uint32_t>::max()}
{
	auto physicalDevice{Graphics::Get()->GetPhysicalDevice()};
	auto surface{Graphics::Get()->GetSurface()};
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};

	auto surfaceFormat{surface->GetFormat()};
	auto surfaceCapabilities{surface->GetCapabilities()};
	auto graphicsFamily{logicalDevice->GetGraphicsFamily()};
	auto presentFamily{logicalDevice->GetPresentFamily()};

	uint32_t physicalPresentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, *surface, &physicalPresentModeCount, nullptr);
	std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, *surface, &physicalPresentModeCount, physicalPresentModes.data());

	for (const auto &presentMode : physicalPresentModes)
	{
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			m_presentMode = presentMode;
			break;
		}

		if (m_presentMode != VK_PRESENT_MODE_MAILBOX_KHR && presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			m_presentMode = presentMode;
		}
	}

	auto desiredImageCount{surfaceCapabilities.minImageCount + 1};

	if (surfaceCapabilities.maxImageCount > 0 && desiredImageCount > surfaceCapabilities.maxImageCount)
	{
		desiredImageCount = surfaceCapabilities.maxImageCount;
	}

	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		// We prefer a non-rotated transform.
		m_preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		m_preTransform = surfaceCapabilities.currentTransform;
	}

	for (const auto &compositeAlphaFlag : COMPOSITE_ALPHA_FLAGS)
	{
		if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag)
		{
			m_compositeAlpha = compositeAlphaFlag;
			break;
		}
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = *surface;
	swapchainCreateInfo.minImageCount = desiredImageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = m_extent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(m_preTransform);
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.compositeAlpha = m_compositeAlpha;
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

	if (oldSwapchain && &oldSwapchain.value() != nullptr)
	{
		swapchainCreateInfo.oldSwapchain = oldSwapchain.value()->m_swapchain;
	}

	if (graphicsFamily != presentFamily)
	{
		std::array<uint32_t, 2> queueFamily{graphicsFamily, presentFamily};
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
		swapchainCreateInfo.pQueueFamilyIndices = queueFamily.data();
	}

	Graphics::CheckVk(vkCreateSwapchainKHR(*logicalDevice, &swapchainCreateInfo, nullptr, &m_swapchain));

	Graphics::CheckVk(vkGetSwapchainImagesKHR(*logicalDevice, m_swapchain, &m_imageCount, nullptr));
	m_images.resize(m_imageCount);
	m_imageViews.resize(m_imageCount);
	Graphics::CheckVk(vkGetSwapchainImagesKHR(*logicalDevice, m_swapchain, &m_imageCount, m_images.data()));

	for (uint32_t i{}; i < m_imageCount; i++)
	{
		Image::CreateImageView(m_images.at(i), m_imageViews.at(i), VK_IMAGE_VIEW_TYPE_2D, surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1, 0);
	}

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkCreateFence(*logicalDevice, &fenceCreateInfo, nullptr, &m_fenceImage);
}

Swapchain::~Swapchain()
{
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};

	vkDestroySwapchainKHR(*logicalDevice, m_swapchain, nullptr);

	for (const auto &imageView : m_imageViews)
	{
		vkDestroyImageView(*logicalDevice, imageView, nullptr);
	}

	vkDestroyFence(*logicalDevice, m_fenceImage, nullptr);
}

VkResult Swapchain::AcquireNextImage(const VkSemaphore &presentCompleteSemaphore)
{
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};

	auto acquireResult{vkAcquireNextImageKHR(*logicalDevice, m_swapchain, std::numeric_limits<uint64_t>::max(), presentCompleteSemaphore, VK_NULL_HANDLE,
		&m_activeImageIndex)};

	if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR && acquireResult != VK_ERROR_OUT_OF_DATE_KHR)
	{
		throw std::runtime_error("Failed to acquire swapchain image");
	}

	//Renderer::CheckVk(vkWaitForFences(*logicalDevice, 1, &m_fenceImage, VK_TRUE, std::numeric_limits<uint64_t>::max()));

	//Renderer::CheckVk(vkResetFences(*logicalDevice, 1, &m_fenceImage));
	return acquireResult;
}

VkResult Swapchain::QueuePresent(const VkQueue &presentQueue, const VkSemaphore &waitSemaphore)
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &waitSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_swapchain;
	presentInfo.pImageIndices = &m_activeImageIndex;
	return vkQueuePresentKHR(presentQueue, &presentInfo);
}
}
