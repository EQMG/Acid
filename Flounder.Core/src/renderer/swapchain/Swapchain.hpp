#pragma once

#include "../../devices/Display.hpp"

namespace Flounder
{
	class Swapchain
	{
	private:
		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_swapChainImages;
		std::vector<VkImageView> m_swapChainImageViews;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;

		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
	public:
		Swapchain();

		~Swapchain();

		void Create(VkSwapChainSupportDetails swapChainSupport);

		void CreateImageViews();

		void CreateFramebuffers(VkRenderPass renderPass);

		void Cleanup();

		void CleanupFramebuffers();

		VkSwapchainKHR GetSwapchain() const { return m_swapChain; }

		size_t GetFramebufferSize() const { return m_swapChainFramebuffers.size(); }

		VkFramebuffer GetFramebuffer(const uint32_t &index) const { return m_swapChainFramebuffers[index]; }

		VkFormat GetImageFormat() const { return m_swapChainImageFormat; }

		VkExtent2D GetExtent() const { return m_swapChainExtent; }
	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	};
}
