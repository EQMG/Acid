﻿#pragma once

#include <vector>
#include "Textures/Texture.hpp"
#include "Swapchain.hpp"

namespace acid
{
	class Renderpass;
	class DepthStencil;

	class ACID_EXPORT Framebuffers :
		public NonCopyable
	{
	public:
		Framebuffers(const uint32_t &width, const uint32_t &height, const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Framebuffers();

		const std::vector<std::unique_ptr<Texture>> &GetImageAttachments() const { return m_imageAttachments; }

		Texture *GetAttachment(const uint32_t &index) const { return m_imageAttachments[index].get(); }

		const std::vector<VkFramebuffer> &GetFramebuffers() const { return m_framebuffers; }
	private:
		std::vector<std::unique_ptr<Texture>> m_imageAttachments;
		std::vector<VkFramebuffer> m_framebuffers;
	};
}
