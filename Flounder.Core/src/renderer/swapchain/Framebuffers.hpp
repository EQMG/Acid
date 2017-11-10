#pragma once

#include <vector>
#include "../../platforms/Platform.hpp"

namespace Flounder
{
	class Framebuffers
	{
	private:
		std::vector<VkFramebuffer> m_framebuffers;
	public:
		Framebuffers(const VkRenderPass &renderPass, const VkImageView &depthImageView, const VkExtent2D &extent, const uint32_t &swapchainImageCount, const std::vector<VkImageView> &swapchinImageViews, const VkImageView &colourImageView, const VkImageView &normalImageView, const VkImageView &shadowImageView);

		~Framebuffers();

		uint32_t GetFramebuffersCount() const { return static_cast<uint32_t>(m_framebuffers.size()); }

		std::vector<VkFramebuffer> GetFramebuffers() const { return m_framebuffers; }
	};
}
