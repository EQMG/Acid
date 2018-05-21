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
		std::vector<VkImage> m_swapchainImages;
		std::vector<VkImageView> m_swapchainImageViews;

		VkExtent2D m_extent;
	public:
		Swapchain(const VkExtent2D &extent);

		~Swapchain();

		VkSwapchainKHR *GetVkSwapchain() { return &m_swapchain; }

		uint32_t GetVkImageCount() const { return m_swapchainImageCount; }

		std::vector<VkImage> GetVkImages() const { return m_swapchainImages; }

		std::vector<VkImageView> GetVkImageViews() const { return m_swapchainImageViews; }

		VkExtent2D GetVkExtent() const { return m_extent; }

		bool IsSameExtent(const VkExtent2D &extent2D) { return m_extent.width == extent2D.width && m_extent.height == extent2D.height; }
	};
}
