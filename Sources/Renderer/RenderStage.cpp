#include "RenderStage.hpp"

#include <algorithm>
#include "Display/Display.hpp"

namespace acid
{
	RenderStage::RenderStage(const uint32_t &stageIndex, const RenderpassCreate &renderpassCreate) :
		m_stageIndex(stageIndex),
		m_renderpassCreate(renderpassCreate),
		m_renderpass(nullptr),
		m_depthStencil(nullptr),
		m_framebuffers(nullptr),
		m_attachments(std::map<std::string, IDescriptor *>()),
		m_clearValues(std::vector<VkClearValue>()),
		m_subpassAttachmentCount(std::vector<uint32_t>(m_renderpassCreate.GetSubpasses().size())),
		m_depthAttachment({}),
		m_swapchainAttachment({}),
		m_subpassMultisampled(std::vector<bool>(m_renderpassCreate.GetSubpasses().size())),
		m_fitDisplaySize(m_renderpassCreate.GetHeight() == 0),
		m_lastWidth(m_renderpassCreate.GetWidth()),
		m_lastHeight(m_renderpassCreate.GetHeight())
	{
		for (auto &image : m_renderpassCreate.GetImages())
		{
			VkClearValue clearValue = {};

			switch (image.GetType())
			{
			case ATTACHMENT_IMAGE:
				clearValue.color = {image.GetClearColour().m_r, image.GetClearColour().m_g, image.GetClearColour().m_b, image.GetClearColour().m_a};

				for (auto &subpass : m_renderpassCreate.GetSubpasses())
				{
					auto subpassBindings = subpass.GetAttachmentBindings();

					if (std::find(subpassBindings.begin(), subpassBindings.end(), image.GetBinding()) != subpassBindings.end())
					{
						m_subpassAttachmentCount[subpass.GetBinding()]++;

						if (image.IsMultisampled())
						{
							m_subpassMultisampled[subpass.GetBinding()] = true;
						}
					}
				}

				break;
			case ATTACHMENT_DEPTH:
				clearValue.depthStencil = {1.0f, 0};
				m_depthAttachment = image;
				break;
			case ATTACHMENT_SWAPCHAIN:
				clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};
				m_swapchainAttachment = image;
				break;
			}

			m_clearValues.emplace_back(clearValue);
		}
	}

	void RenderStage::Rebuild(const Swapchain &swapchain)
	{
#if defined(ACID_VERBOSE)
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		auto msaaSamples = Display::Get()->GetMsaaSamples();

		if (m_depthAttachment)
		{
			m_depthStencil = std::make_unique<DepthStencil>(GetWidth(), GetHeight(), m_depthAttachment->IsMultisampled() ? msaaSamples : VK_SAMPLE_COUNT_1_BIT);
		}

		if (m_renderpass == nullptr)
		{
			m_renderpass = std::make_unique<Renderpass>(m_renderpassCreate, *m_depthStencil, surfaceFormat.format, msaaSamples);
		}

		m_framebuffers = std::make_unique<Framebuffers>(GetWidth(), GetHeight(), m_renderpassCreate, *m_renderpass, swapchain, *m_depthStencil, msaaSamples);

		m_attachments.clear();

		for (auto &image : m_renderpassCreate.GetImages())
		{
			if (image.GetType() == ATTACHMENT_DEPTH)
			{
				m_attachments.emplace(image.GetName(), m_depthStencil.get());
			}
			else
			{
				m_attachments.emplace(image.GetName(), m_framebuffers->GetAttachment(image.GetBinding()));
			}
		}

#if defined(ACID_VERBOSE)
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Renderstage '%i' built in %fms\n", m_stageIndex, debugEnd - debugStart);
#endif
	}

	uint32_t RenderStage::GetWidth() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate.GetWidth();
		}

		return Display::Get()->GetWidth();
	}

	uint32_t RenderStage::GetHeight() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate.GetHeight();
		}

		return Display::Get()->GetHeight();
	}

	bool RenderStage::IsOutOfDate(const VkExtent2D &extent2D)
	{
		uint32_t currentWidth = GetWidth();
		uint32_t currentHeight = GetHeight();
		bool outOfDate = currentWidth != m_lastWidth || currentHeight != m_lastHeight;
		m_lastWidth = currentWidth;
		m_lastHeight = currentHeight;
		return outOfDate;
	}

	IDescriptor *RenderStage::GetAttachment(const std::string &name) const
	{
		auto it = m_attachments.find(name);

		if (it != m_attachments.end())
		{
			return it->second;
		}

		return nullptr;
	}

	VkFramebuffer RenderStage::GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const
	{
		if (activeSwapchainImage > m_framebuffers->GetFramebuffers().size())
		{
			return m_framebuffers->GetFramebuffers().at(0);
		}

		return m_framebuffers->GetFramebuffers().at(activeSwapchainImage);
	}
}
