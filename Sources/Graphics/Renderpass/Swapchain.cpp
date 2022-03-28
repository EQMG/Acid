#include "Swapchain.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
static const std::vector<VkCompositeAlphaFlagBitsKHR> COMPOSITE_ALPHA_FLAGS = {
	VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
	VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR, VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
};

Swapchain::Swapchain(const PhysicalDevice &physicalDevice, const Surface &surface, const LogicalDevice &logicalDevice, const VkExtent2D &extent, const Swapchain *oldSwapchain) :
	physicalDevice(physicalDevice),
	surface(surface),
	logicalDevice(logicalDevice),
	extent(extent),
	presentMode(VK_PRESENT_MODE_FIFO_KHR),
	preTransform(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR),
	compositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR),
	activeImageIndex(std::numeric_limits<uint32_t>::max()) {
	auto surfaceFormat = surface.GetFormat();
	auto surfaceCapabilities = surface.GetCapabilities();
	auto graphicsFamily = logicalDevice.GetGraphicsFamily();
	auto presentFamily = logicalDevice.GetPresentFamily();

	uint32_t physicalPresentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount, nullptr);
	std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount, physicalPresentModes.data());

	for (const auto &presentMode : physicalPresentModes) {
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			this->presentMode = presentMode;
			break;
		}

		if (presentMode != VK_PRESENT_MODE_MAILBOX_KHR && presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			this->presentMode = presentMode;
		}
	}

	auto desiredImageCount = surfaceCapabilities.minImageCount + 1;

	if (surfaceCapabilities.maxImageCount > 0 && desiredImageCount > surfaceCapabilities.maxImageCount) {
		desiredImageCount = surfaceCapabilities.maxImageCount;
	}

	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		// We prefer a non-rotated transform.
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	} else {
		preTransform = surfaceCapabilities.currentTransform;
	}

	for (const auto &compositeAlphaFlag : COMPOSITE_ALPHA_FLAGS) {
		if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag) {
			compositeAlpha = compositeAlphaFlag;
			break;
		}
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = desiredImageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = this->extent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform);
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.compositeAlpha = compositeAlpha;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	if (oldSwapchain)
		swapchainCreateInfo.oldSwapchain = oldSwapchain->swapchain;

	if (graphicsFamily != presentFamily) {
		std::array<uint32_t, 2> queueFamily = {graphicsFamily, presentFamily};
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
		swapchainCreateInfo.pQueueFamilyIndices = queueFamily.data();
	}

	Graphics::CheckVk(vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &swapchain));

	Graphics::CheckVk(vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr));
	images.resize(imageCount);
	imageViews.resize(imageCount);
	Graphics::CheckVk(vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, images.data()));

	for (uint32_t i = 0; i < imageCount; i++) {
		Image::CreateImageView(images.at(i), imageViews.at(i), VK_IMAGE_VIEW_TYPE_2D, surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, 
			1, 0, 1, 0);
	}

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &fenceImage);
}

Swapchain::~Swapchain() {
	vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);

	for (const auto &imageView : imageViews) {
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	}

	vkDestroyFence(logicalDevice, fenceImage, nullptr);
}

VkResult Swapchain::AcquireNextImage(const VkSemaphore &presentCompleteSemaphore, VkFence fence) {
	if (fence != VK_NULL_HANDLE)
		Graphics::CheckVk(vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()));
	
	auto acquireResult = vkAcquireNextImageKHR(logicalDevice, swapchain, std::numeric_limits<uint64_t>::max(), presentCompleteSemaphore, VK_NULL_HANDLE,
		&activeImageIndex);

	if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR && acquireResult != VK_ERROR_OUT_OF_DATE_KHR)
		throw std::runtime_error("Failed to acquire swapchain image");

	//Graphics::CheckVk(vkWaitForFences(*logicalDevice, 1, &fenceImage, VK_TRUE, std::numeric_limits<uint64_t>::max()));

	//Graphics::CheckVk(vkResetFences(*logicalDevice, 1, &fenceImage));
	return acquireResult;
}

VkResult Swapchain::QueuePresent(const VkQueue &presentQueue, const VkSemaphore &waitSemaphore) {
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &waitSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &activeImageIndex;
	return vkQueuePresentKHR(presentQueue, &presentInfo);
}
}
