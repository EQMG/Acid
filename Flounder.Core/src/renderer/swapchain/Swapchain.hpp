#pragma once

#include <vector>

#include "../../platforms/glfw/GlfwVulkan.h"

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
		std::vector<VkImage> m_swapchainImages;
		// Handles to all image views associated with swapChainImages.
		std::vector<VkImageView> m_swapchainImageViews;
		// Vector of handles to framebuffers.
		std::vector<VkFramebuffer> m_swapchainFramebuffers;

		// Stores the chosen image format.
		VkFormat m_swapchainImageFormat;
		// Stores the chosen image extent.
		VkExtent2D m_swapchainExtent;

		const VkDevice *m_device;
	public:
		Swapchain();

		~Swapchain();

		void Create(const VkDevice *logicalDevice, VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, GLFWwindow *window);

		void CreateFramebuffers(const VkDevice* logicalDevice, const VkRenderPass* renderPass);

		void Cleanup();

		void CleanupFrameBuffers();

		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		
		VkSwapchainKHR GetSwapchain() const { return m_swapchain; }

		size_t GetFramebufferSize() const { return m_swapchainFramebuffers.size(); }

		VkFramebuffer GetFramebuffer(const uint32_t &index) const { return m_swapchainFramebuffers[index]; }

		VkFormat GetImageFormat() const { return m_swapchainImageFormat; }

		VkExtent2D GetExtent() const { return m_swapchainExtent; }
	private:
		void CreateImageViews(const VkDevice* logicalDevice);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const;

		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const;

		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) const;
	};
}
