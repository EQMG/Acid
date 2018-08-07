#pragma once

#include <vector>
#include "Swapchain.hpp"

namespace acid
{
	class Renderpass;

	class DepthStencil;

	class ACID_EXPORT Framebuffers
	{
	private:
		std::vector<Texture *> m_imageAttachments;
		std::vector<VkFramebuffer> m_framebuffers;
	public:
		Framebuffers(const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkExtent2D &extent, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Framebuffers();

		std::vector<Texture *> GetImageAttachments() const { return m_imageAttachments; }

		Texture *GetTexture(const uint32_t &i) const { return m_imageAttachments.at(i); }

		std::vector<VkFramebuffer> GetFramebuffers() const { return m_framebuffers; }
	};
}
