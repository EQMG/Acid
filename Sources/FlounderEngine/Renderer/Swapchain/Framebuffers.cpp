#include "Framebuffers.hpp"

#include "../Renderpass/Renderpass.hpp"
#include "DepthStencil.hpp"

namespace Flounder
{
	Framebuffers::Framebuffers(const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkExtent2D &extent) :
		m_imageAttachments(std::vector<Texture *>()),
		m_framebuffers(std::vector<VkFramebuffer>())
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		uint32_t width = renderpassCreate.m_width == 0 ? Display::Get()->GetWidth() : renderpassCreate.m_width;
		uint32_t height = renderpassCreate.m_height == 0 ? Display::Get()->GetHeight() : renderpassCreate.m_height;

		for (auto image : renderpassCreate.images)
		{
			switch (image.m_type)
			{
			case TypeImage:
				m_imageAttachments.push_back(new Texture(width, height, image.m_format, image.m_layout, image.m_usage));
				break;
			case TypeDepth:
				m_imageAttachments.push_back(nullptr);
				break;
			case TypeSwapchain:
				m_imageAttachments.push_back(nullptr);
				break;
			}
		}

		m_framebuffers.resize(swapchain.GetImageCount());

		for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
		{
			std::vector<VkImageView> attachments = {};

			for (auto image : renderpassCreate.images)
			{
				switch (image.m_type)
				{
				case TypeImage:
					attachments.push_back(GetTexture(image.m_binding)->GetImageView());
					break;
				case TypeDepth:
					attachments.push_back(depthStencil.GetImageView());
					break;
				case TypeSwapchain:
					attachments.push_back(swapchain.GetImageViews().at(i));
					break;
				}
			}

			VkFramebufferCreateInfo framebufferCreateInfo = {};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = renderPass.GetRenderpass();
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferCreateInfo.pAttachments = attachments.data();
			framebufferCreateInfo.width = extent.width;
			framebufferCreateInfo.height = extent.height;
			framebufferCreateInfo.layers = 1;

			Display::ErrorVk(vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &m_framebuffers.at(i)));
		}
	}

	Framebuffers::~Framebuffers()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto attachment : m_imageAttachments)
		{
			delete attachment;
		}

		for (auto framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
		}
	}
}
