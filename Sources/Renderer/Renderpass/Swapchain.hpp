#pragma once

#include "Renderer/Renderpass/RenderpassCreate.hpp"

namespace acid
{
	class ACID_EXPORT Swapchain
	{
	public:
		explicit Swapchain(const VkExtent2D &extent);

		~Swapchain();

		/// <summary>
		/// Acquires the next image in the swapchain into the internal acquired image. The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX.
		/// </summary>
		/// <param name="presentCompleteSemaphore"> A optional semaphore that is signaled when the image is ready for use. </param>
		/// <returns> Result of the image acquisition. </returns>
		VkResult AcquireNextImage(const VkSemaphore &presentCompleteSemaphore = VK_NULL_HANDLE);

		/// <summary>
		/// Queue an image for presentation using the internal acquired image for queue presentation.
		/// </summary>
		/// <param name="presentQueue"> Presentation queue for presenting the image. </param>
		/// <param name="waitSemaphore"> A optional semaphore that is waited on before the image is presented. </param>
		/// <returns> Result of the queue presentation. </returns>
		VkResult QueuePresent(const VkQueue &presentQueue, const VkSemaphore &waitSemaphore = VK_NULL_HANDLE);

		const VkExtent2D &GetExtent() const { return m_extent; }

		const uint32_t &GetImageCount() const { return m_imageCount; }

		const VkSurfaceTransformFlagsKHR &GetPreTransform() const { return m_preTransform; }

		const VkCompositeAlphaFlagBitsKHR &GetCompositeAlpha() const { return m_compositeAlpha; }

		const std::vector<VkImage> &GetImages() const { return m_images; }

		const VkImage &GetActiveImage() const { return m_images[m_activeImageIndex]; }

		const std::vector<VkImageView> &GetImageViews() const { return m_imageViews; }

		const VkSwapchainKHR &GetSwapchain() const { return m_swapchain; }

		const uint32_t &GetActiveImageIndex() const { return m_activeImageIndex; }

		bool IsSameExtent(const VkExtent2D &extent2D) { return m_extent.width == extent2D.width && m_extent.height == extent2D.height; }
	private:
		VkExtent2D m_extent;
		VkPresentModeKHR m_presentMode;

		uint32_t m_imageCount;
		VkSurfaceTransformFlagsKHR m_preTransform;
		VkCompositeAlphaFlagBitsKHR m_compositeAlpha;
		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_imageViews;
		VkSwapchainKHR m_swapchain;

		VkFence m_fenceImage;
		uint32_t m_activeImageIndex;
	};
}
