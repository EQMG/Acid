#pragma once

#include <vector>

#include "../../platforms/glfw/Platform.hpp"

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

		VkExtent2D m_extent;
	public:
		Swapchain(const VkExtent2D &extent);

		~Swapchain();

		VkSwapchainKHR *GetSwapchain() { return &m_swapchain; }

		uint32_t GetImageCount() const { return m_swapchainImageCount; }

		std::vector<VkImageView> GetImageViews() const { return m_swapchinImageViews; }

		VkExtent2D GetExtent() const { return m_extent; }
	};
}
