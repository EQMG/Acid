#pragma once

#include <optional>
#include <vector>
#include <volk.h>

#include "Utils/NonCopyable.hpp"
#include "GraphicsExport.hpp"

namespace acid {
class LogicalDevice;
class ImageDepth;
class RenderStage;

class ACID_GRAPHICS_EXPORT Renderpass {
public:
	class SubpassDescription : NonCopyable {
	public:
		SubpassDescription(VkPipelineBindPoint bindPoint, std::vector<VkAttachmentReference> colorAttachments, const std::optional<uint32_t> &depthAttachment) :
			colorAttachments(std::move(colorAttachments)) {
			subpassDescription.pipelineBindPoint = bindPoint;
			subpassDescription.colorAttachmentCount = static_cast<uint32_t>(this->colorAttachments.size());
			subpassDescription.pColorAttachments = this->colorAttachments.data();

			if (depthAttachment) {
				depthStencilAttachment.attachment = *depthAttachment;
				depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				subpassDescription.pDepthStencilAttachment = &depthStencilAttachment;
			}
		}

		const VkSubpassDescription &GetSubpassDescription() const { return subpassDescription; }

	private:
		VkSubpassDescription subpassDescription = {};
		std::vector<VkAttachmentReference> colorAttachments;
		VkAttachmentReference depthStencilAttachment = {};
	};

	Renderpass(const LogicalDevice &logicalDevice, const RenderStage &renderStage, VkFormat depthFormat, VkFormat surfaceFormat, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
	~Renderpass();

	operator const VkRenderPass &() const { return renderpass; }
	const VkRenderPass &GetRenderpass() const { return renderpass; }

private:
	const LogicalDevice &logicalDevice;

	VkRenderPass renderpass = VK_NULL_HANDLE;
};
}
