#pragma once

#include <vector>
#include "Textures/Texture.hpp"
#include "Renderer/Renderpass/RenderpassCreate.hpp"

namespace fl
{
	class FL_EXPORT Swapchain
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

		std::vector<VkImage> GetImages() const { return m_swapchinImages; }

		std::vector<VkImageView> GetImageViews() const { return m_swapchinImageViews; }

		VkExtent2D GetExtent() const { return m_extent; }

		bool SameExtent(const VkExtent2D &extent2D) { return m_extent.width == extent2D.width && m_extent.height == extent2D.height; }
	};
}
