#pragma once

#include <vector>
#include "../../platforms/Platform.hpp"
#include "Swapchain.hpp"

namespace Flounder
{
	class Framebuffers
	{
	private:
		std::vector<VkFramebuffer> m_framebuffers;
	public:
		Framebuffers(const VkRenderPass &renderPass, const VkImageView &depthImageView, const Swapchain &swapchain, const VkExtent2D &extent);

		~Framebuffers();

		uint32_t GetFramebuffersCount() const { return static_cast<uint32_t>(m_framebuffers.size()); }

		std::vector<VkFramebuffer> GetFramebuffers() const { return m_framebuffers; }
	};
}
