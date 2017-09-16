#pragma once

#include <vector>

#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	/// <summary>
	/// Used to store data for the swap chain.
	/// </summary>
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Swapchain
	{
	private:
		// The swapchain handler.
		VkSwapchainKHR m_swapchain;
		// Vector of swap chain images.
		std::vector<VkImage> m_images;
		// Handles to all image views associated with swapChainImages.
		std::vector<VkImageView> m_imageViews;
		// Vector of handles to framebuffers.
		std::vector<VkFramebuffer> m_framebuffers;

		// Stores the chosen image format.
		VkFormat m_imageFormat;
		// Stores the chosen image extent.
		VkExtent2D m_extent;
	public:
		Swapchain();

		~Swapchain();

		void Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, GLFWwindow *window);

		void CreateFramebuffers(const VkDevice *logicalDevice, const VkRenderPass &renderPass);

		void Cleanup(const VkDevice *logicalDevice);

		void CleanupFramebuffers(const VkDevice *logicalDevice);

		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		
		VkSwapchainKHR GetSwapchain() const { return m_swapchain; }

		VkFormat GetImageFormat() const { return m_imageFormat; }

		size_t GetFramebufferSize() const { return m_framebuffers.size(); }

		VkFramebuffer GetFramebuffer(const uint32_t &index) const { return m_framebuffers[index]; }

		std::vector<VkImageView> GetImageViews() const { return m_imageViews; }

		VkExtent2D GetExtent() const { return m_extent; }
	private:
		void CreateImageViews(const VkDevice* logicalDevice);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const;

		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const;

		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) const;
	};
}
