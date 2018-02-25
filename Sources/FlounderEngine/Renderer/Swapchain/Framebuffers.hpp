﻿#pragma once

#include <vector>
#include "../../Engine/Platform.hpp"
#include "Swapchain.hpp"

namespace Flounder
{
	class Renderpass;
	class DepthStencil;

	class F_EXPORT Framebuffers
	{
	private:
		std::vector<Texture *> m_imageAttachments;
		std::vector<VkFramebuffer> m_framebuffers;
	public:
		Framebuffers(const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkExtent2D &extent);

		~Framebuffers();

		std::vector<Texture *> GetImageAttachments() const { return m_imageAttachments; }

		Texture *GetTexture(const unsigned int &i) const { return m_imageAttachments.at(i); }

		uint32_t GetFramebuffersCount() const { return static_cast<uint32_t>(m_framebuffers.size()); }

		std::vector<VkFramebuffer> GetFramebuffers() const { return m_framebuffers; }
	};
}
