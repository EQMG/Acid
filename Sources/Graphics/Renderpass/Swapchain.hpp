#pragma once

#include <volk.h>
#include <vector>

#include "Export.hpp"

namespace acid {
class ACID_EXPORT Swapchain {
public:
	explicit Swapchain(const VkExtent2D &extent, const Swapchain *oldSwapchain = nullptr);
	
	~Swapchain();

	/**
	 * Acquires the next image in the swapchain into the internal acquired image. The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX.
	 * @param presentCompleteSemaphore A optional semaphore that is signaled when the image is ready for use.
	 * @param fence A optional fence that is signaled once the previous command buffer has completed.
	 * @return Result of the image acquisition.
	 */
	VkResult AcquireNextImage(const VkSemaphore &presentCompleteSemaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE);

	/**
	 * Queue an image for presentation using the internal acquired image for queue presentation.
	 * @param presentQueue Presentation queue for presenting the image.
	 * @param waitSemaphore A optional semaphore that is waited on before the image is presented.
	 * @return Result of the queue presentation.
	 */
	VkResult QueuePresent(const VkQueue &presentQueue, const VkSemaphore &waitSemaphore = VK_NULL_HANDLE);

	bool IsSameExtent(const VkExtent2D &extent2D) { return extent.width == extent2D.width && extent.height == extent2D.height; }

	operator const VkSwapchainKHR &() const { return swapchain; }

	const VkExtent2D &GetExtent() const { return extent; }
	uint32_t GetImageCount() const { return imageCount; }
	VkSurfaceTransformFlagsKHR GetPreTransform() const { return preTransform; }
	VkCompositeAlphaFlagBitsKHR GetCompositeAlpha() const { return compositeAlpha; }
	const std::vector<VkImage> &GetImages() const { return images; }
	const VkImage &GetActiveImage() const { return images[activeImageIndex]; }
	const std::vector<VkImageView> &GetImageViews() const { return imageViews; }
	const VkSwapchainKHR &GetSwapchain() const { return swapchain; }
	uint32_t GetActiveImageIndex() const { return activeImageIndex; }

private:
	VkExtent2D extent;
	VkPresentModeKHR presentMode;

	uint32_t imageCount = 0;
	VkSurfaceTransformFlagsKHR preTransform;
	VkCompositeAlphaFlagBitsKHR compositeAlpha;
	std::vector<VkImage> images;
	std::vector<VkImageView> imageViews;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;

	VkFence fenceImage = VK_NULL_HANDLE;
	uint32_t activeImageIndex;
};
}
