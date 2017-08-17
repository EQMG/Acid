#pragma once

#include "../../devices/display.h"

namespace flounder
{
	class swapchain
	{
	private:
		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_swapChainImages;
		std::vector<VkImageView> m_swapChainImageViews;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;

		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
	public:
		swapchain();

		~swapchain();

		void create(VkSwapChainSupportDetails swapChainSupport);

		void createImageViews();

		void createFramebuffers(VkRenderPass renderPass);

		void cleanup();

		void cleanupFramebuffers();

		inline VkSwapchainKHR getSwapchain() const { return m_swapChain; }

		inline size_t getFramebufferSize() const { return m_swapChainFramebuffers.size(); }

		inline VkFramebuffer getFramebuffer(uint32_t index) const { return m_swapChainFramebuffers[index]; }

		inline VkFormat getImageFormat() const { return m_swapChainImageFormat; }

		inline VkExtent2D getExtent() const { return m_swapChainExtent; }
	private:
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	};
}
