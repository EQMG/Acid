﻿#pragma once

#include <vector>
#include "Renderer/Renderpass/RenderpassCreate.hpp"

namespace acid
{
	class ACID_EXPORT Swapchain
	{
	private:
		VkPresentModeKHR m_presentMode;
		VkSwapchainKHR m_swapchain;
		uint32_t m_swapchainImageCount;
		std::vector<VkImage> m_swapchainImages;
		std::vector<VkImageView> m_swapchainImageViews;

		VkExtent2D m_extent;
	public:
		explicit Swapchain(const VkExtent2D &extent);

		~Swapchain();

		const VkSwapchainKHR &GetSwapchain() const { return m_swapchain; }

		const uint32_t &GetImageCount() const { return m_swapchainImageCount; }

		const std::vector<VkImage> &GetImages() const { return m_swapchainImages; }

		const std::vector<VkImageView> &GetImageViews() const { return m_swapchainImageViews; }

		const VkExtent2D &GetExtent() const { return m_extent; }

		bool IsSameExtent(const VkExtent2D &extent2D) { return m_extent.width == extent2D.width && m_extent.height == extent2D.height; }
	};
}
