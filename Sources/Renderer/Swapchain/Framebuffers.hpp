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
		std::vector<std::shared_ptr<Texture>> m_imageAttachments;

		std::vector<VkFramebuffer> m_framebuffers;
	public:
		Framebuffers(const uint32_t &width, const uint32_t &height, const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Framebuffers();

		std::vector<std::shared_ptr<Texture>> GetImageAttachments() const { return m_imageAttachments; }

		std::shared_ptr<Texture> GetAttachment(const uint32_t &index) const { return m_imageAttachments.at(index); }

		std::vector<VkFramebuffer> GetFramebuffers() const { return m_framebuffers; }
	};
}
