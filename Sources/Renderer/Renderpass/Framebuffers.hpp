#pragma once

#include "Images/Image2d.hpp"
#include "Swapchain.hpp"

namespace acid
{
class Renderpass;
class ImageDepth;

class ACID_EXPORT Framebuffers :
	public NonCopyable
{
public:
	Framebuffers(const uint32_t &width, const uint32_t &height, const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const ImageDepth &depthStencil,
		const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

	~Framebuffers();

	const std::vector<std::unique_ptr<Image2d>> &GetImageAttachments() const { return m_imageAttachments; }

	Image2d *GetAttachment(const uint32_t &index) const { return m_imageAttachments[index].get(); }

	const std::vector<VkFramebuffer> &GetFramebuffers() const { return m_framebuffers; }

private:
	std::vector<std::unique_ptr<Image2d>> m_imageAttachments;
	std::vector<VkFramebuffer> m_framebuffers;
};
}
