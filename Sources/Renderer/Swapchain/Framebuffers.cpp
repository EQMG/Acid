#include "Framebuffers.hpp"

#include "Display/Display.hpp"
#include "Renderer/Renderpass/Renderpass.hpp"
#include "DepthStencil.hpp"

namespace acid
{
	Framebuffers::Framebuffers(const uint32_t &width, const uint32_t &height, const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkSampleCountFlagBits &samples) :
		m_imageAttachments(std::vector<std::unique_ptr<Texture>>()),
		m_framebuffers(std::vector<VkFramebuffer>())
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto textureWidth = renderpassCreate.GetWidth() == 0 ? Display::Get()->GetWidth() : renderpassCreate.GetWidth();
		auto textureHeight = renderpassCreate.GetHeight() == 0 ? Display::Get()->GetHeight() : renderpassCreate.GetHeight();

		for (auto &image : renderpassCreate.GetImages())
		{
			auto imageSamples = image.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;

			switch (image.GetType())
			{
			case ATTACHMENT_TYPE_IMAGE:
				m_imageAttachments.emplace_back(std::make_unique<Texture>(textureWidth, textureHeight, nullptr, image.GetFormat(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, imageSamples));
				break;
			case ATTACHMENT_TYPE_DEPTH:
				m_imageAttachments.emplace_back(nullptr);
				break;
			case ATTACHMENT_TYPE_SWAPCHAIN:
				m_imageAttachments.emplace_back(nullptr);
				break;
			}
		}

		m_framebuffers.resize(swapchain.GetImageCount());

		for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
		{
			std::vector<VkImageView> attachments = {};

			for (auto &image : renderpassCreate.GetImages())
			{
				switch (image.GetType())
				{
				case ATTACHMENT_TYPE_IMAGE:
					attachments.emplace_back(GetAttachment(image.GetBinding())->GetImageView());
					break;
				case ATTACHMENT_TYPE_DEPTH:
					attachments.emplace_back(depthStencil.GetImageView());
					break;
				case ATTACHMENT_TYPE_SWAPCHAIN:
					attachments.emplace_back(swapchain.GetImageViews().at(i));
					break;
				}
			}

			VkFramebufferCreateInfo framebufferCreateInfo = {};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = renderPass.GetRenderpass();
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferCreateInfo.pAttachments = attachments.data();
			framebufferCreateInfo.width = width;
			framebufferCreateInfo.height = height;
			framebufferCreateInfo.layers = 1;
			Display::CheckVk(vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &m_framebuffers.at(i)));
		}
	}

	Framebuffers::~Framebuffers()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto &framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
		}
	}
}
