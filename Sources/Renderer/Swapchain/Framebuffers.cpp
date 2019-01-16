#include "Framebuffers.hpp"

#include "Renderer/Renderer.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Renderpass/Renderpass.hpp"
#include "Renderer/Renderer.hpp"
#include "Textures/DepthStencil.hpp"

namespace acid
{
	Framebuffers::Framebuffers(const uint32_t &width, const uint32_t &height, const RenderpassCreate &renderpassCreate, const Renderpass &renderPass,
		const Swapchain &swapchain, const DepthStencil &depthStencil, const VkSampleCountFlagBits &samples) :
		m_imageAttachments(std::vector<std::unique_ptr<Texture>>()),
		m_framebuffers(std::vector<VkFramebuffer>())
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		auto textureWidth = renderpassCreate.GetWidth() == 0 ? Window::Get()->GetWidth() : renderpassCreate.GetWidth();
		auto textureHeight = renderpassCreate.GetHeight() == 0 ? Window::Get()->GetHeight() : renderpassCreate.GetHeight();

		for (const auto &image : renderpassCreate.GetImages())
		{
			auto imageSamples = image.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;

			switch (image.GetType())
			{
			case Attachment::Type::Image:
				m_imageAttachments.emplace_back(std::make_unique<Texture>(textureWidth, textureHeight, nullptr, image.GetFormat(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, imageSamples));
				break;
			case Attachment::Type::Depth:
				m_imageAttachments.emplace_back(nullptr);
				break;
			case Attachment::Type::Swapchain:
				m_imageAttachments.emplace_back(nullptr);
				break;
			}
		}

		m_framebuffers.resize(swapchain.GetImageCount());

		for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
		{
			std::vector<VkImageView> attachments = {};

			for (const auto &image : renderpassCreate.GetImages())
			{
				switch (image.GetType())
				{
				case Attachment::Type::Image:
					attachments.emplace_back(GetAttachment(image.GetBinding())->GetImageView());
					break;
				case Attachment::Type::Depth:
					attachments.emplace_back(depthStencil.GetImageView());
					break;
				case Attachment::Type::Swapchain:
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
			Renderer::CheckVk(vkCreateFramebuffer(logicalDevice->GetLogicalDevice(), &framebufferCreateInfo, nullptr, &m_framebuffers.at(i)));
		}
	}

	Framebuffers::~Framebuffers()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		for (const auto &framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice->GetLogicalDevice(), framebuffer, nullptr);
		}
	}
}
