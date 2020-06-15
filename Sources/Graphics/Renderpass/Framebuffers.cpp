#include "Framebuffers.hpp"

#include "Graphics/Images/ImageDepth.hpp"
#include "Graphics/Renderpass/Renderpass.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/RenderStage.hpp"

namespace acid {
Framebuffers::Framebuffers(const Vector2ui &extent, const RenderStage &renderStage, const Renderpass &renderPass, const Swapchain &swapchain, const ImageDepth &depthStencil,
	VkSampleCountFlagBits samples) {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	for (const auto &attachment : renderStage.GetAttachments()) {
		auto attachmentSamples = attachment.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;

		switch (attachment.GetType()) {
		case Attachment::Type::Image:
			imageAttachments.emplace_back(std::make_unique<Image2d>(extent, attachment.GetFormat(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, attachmentSamples));
			break;
		case Attachment::Type::Depth:
			imageAttachments.emplace_back(nullptr);
			break;
		case Attachment::Type::Swapchain:
			imageAttachments.emplace_back(nullptr);
			break;
		}
	}

	framebuffers.resize(swapchain.GetImageCount());

	for (uint32_t i = 0; i < swapchain.GetImageCount(); i++) {
		std::vector<VkImageView> attachments;

		for (const auto &attachment : renderStage.GetAttachments()) {
			switch (attachment.GetType()) {
			case Attachment::Type::Image:
				attachments.emplace_back(GetAttachment(attachment.GetBinding())->GetView());
				break;
			case Attachment::Type::Depth:
				attachments.emplace_back(depthStencil.GetView());
				break;
			case Attachment::Type::Swapchain:
				attachments.emplace_back(swapchain.GetImageViews().at(i));
				break;
			}
		}

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass;
		framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferCreateInfo.pAttachments = attachments.data();
		framebufferCreateInfo.width = extent.x;
		framebufferCreateInfo.height = extent.y;
		framebufferCreateInfo.layers = 1;
		Graphics::CheckVk(vkCreateFramebuffer(*logicalDevice, &framebufferCreateInfo, nullptr, &framebuffers[i]));
	}
}

Framebuffers::~Framebuffers() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	for (const auto &framebuffer : framebuffers)
		vkDestroyFramebuffer(*logicalDevice, framebuffer, nullptr);
}
}
