#pragma once

#include <vector>
#include "Swapchain.hpp"

namespace fl
{
	class Renderpass;

	class DepthStencil;

	class FL_EXPORT Framebuffers
	{
	private:
		std::vector<Texture *> m_imageAttachments;
		std::vector<VkFramebuffer> m_framebuffers;
	public:
		Framebuffers(const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkExtent2D &extent);

		~Framebuffers();

		std::vector<Texture *> GetImageAttachments() const { return m_imageAttachments; }

		Texture *GetTexture(const unsigned int &i) const { return m_imageAttachments.at(i); }

		std::vector<VkFramebuffer> GetVkFramebuffers() const { return m_framebuffers; }
	};
}
