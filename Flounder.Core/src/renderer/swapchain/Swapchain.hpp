#pragma once

#include <vector>

#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	class Swapchain
	{
	private:
		VkPresentModeKHR m_presentMode;
		VkSwapchainKHR m_swapchain;
		uint32_t m_swapchainImageCount;
		std::vector<VkImage> m_swapchinImages;
		std::vector<VkImageView> m_swapchinImageViews;

//		VkFormat m_imageFormat;
		VkExtent2D m_extent;

		std::vector<VkFramebuffer> m_framebuffers;
	public:
		Swapchain();

		~Swapchain();

		void Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkSurfaceCapabilitiesKHR &surfaceCapabilities, const VkSurfaceFormatKHR &surfaceFormat, const VkExtent2D &extent);

		void CreateFrameBuffers(const VkDevice &logicalDevice, const VkRenderPass &renderPass, const VkImageView &depthImageView, const VkExtent2D &extent);

		void Cleanup(const VkDevice &logicalDevice);

		void CleanupFrameBuffers(const VkDevice &device);

		VkSwapchainKHR *GetSwapchain() { return &m_swapchain; }

		uint32_t GetImageCount() { return m_swapchainImageCount; }

		std::vector<VkImageView> GetImageViews() { return m_swapchinImageViews; }

		VkExtent2D GetExtent() const { return m_extent; }

	//	VkFormat GetImageFormat() const { return m_imageFormat; }

		uint32_t GetFramebuffersCount() const { return static_cast<uint32_t>(m_framebuffers.size()); }

		std::vector<VkFramebuffer> GetFramebuffers() const { return m_framebuffers; }
	};
}
