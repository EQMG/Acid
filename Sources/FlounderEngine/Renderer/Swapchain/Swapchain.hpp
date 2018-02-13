#pragma once

#include <vector>
#include "../../Engine/Platform.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	class F_EXPORT Swapchain
	{
	private:
		VkPresentModeKHR m_presentMode;
		VkSwapchainKHR m_swapchain;
		uint32_t m_swapchainImageCount;
		std::vector<VkImage> m_swapchinImages;
		std::vector<VkImageView> m_swapchinImageViews;
		Texture *m_colourImage;
		Texture *m_normalImage;
		Texture *m_materialImage;
		Texture *m_shadowImage;

		VkExtent2D m_extent;
	public:
		Swapchain(const VkExtent2D &extent);

		~Swapchain();

		VkSwapchainKHR *GetSwapchain() { return &m_swapchain; }

		uint32_t GetImageCount() const { return m_swapchainImageCount; }

		std::vector<VkImage> GetImages() const { return m_swapchinImages; }

		std::vector<VkImageView> GetImageViews() const { return m_swapchinImageViews; }

		Texture *GetColourImage() const { return m_colourImage; }

		Texture *GetNormalImage() const { return m_normalImage; }

		Texture *GetMaterialImage() const { return m_materialImage; }

		Texture *GetShadowImage() const { return m_shadowImage; }

		VkExtent2D GetExtent() const { return m_extent; }
	};
}
