#pragma once

#include "Graphics/Images/Image2d.hpp"
#include "Swapchain.hpp"

namespace acid {
class LogicalDevice;
class ImageDepth;
class Renderpass;
class RenderStage;

class ACID_EXPORT Framebuffers : NonCopyable {
public:
	Framebuffers(const LogicalDevice &logicalDevice, const Swapchain &swapchain, const RenderStage &renderStage, const Renderpass &renderPass, const ImageDepth &depthStencil,
		const Vector2ui &extent, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
	~Framebuffers();

	Image2d *GetAttachment(uint32_t index) const { return imageAttachments[index].get(); }

	const std::vector<std::unique_ptr<Image2d>> &GetImageAttachments() const { return imageAttachments; }
	const std::vector<VkFramebuffer> &GetFramebuffers() const { return framebuffers; }

private:
	const LogicalDevice &logicalDevice;

	std::vector<std::unique_ptr<Image2d>> imageAttachments;
	std::vector<VkFramebuffer> framebuffers;
};
}
