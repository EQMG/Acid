#include "RenderStage.hpp"

#include <algorithm>
#include "Display/Display.hpp"

namespace acid
{
	RenderStage::RenderStage(const uint32_t &stageIndex, RenderpassCreate *renderpassCreate) :
		m_stageIndex(stageIndex),
		m_renderpassCreate(renderpassCreate),
		m_depthStencil(nullptr),
		m_renderpass(nullptr),
		m_framebuffers(nullptr),
		m_clearValues(std::vector<VkClearValue>()),
		m_subpassAttachmentCount(std::vector<uint32_t>(renderpassCreate->GetSubpasses().size())),
		m_hasDepth(false),
		m_hasSwapchain(false),
		m_fitDisplaySize(m_renderpassCreate->GetHeight() == 0),
		m_lastWidth(renderpassCreate->GetWidth()),
		m_lastHeight(renderpassCreate->GetHeight())
	{
		for (auto &image : renderpassCreate->GetImages())
		{
			VkClearValue clearValue = {};

			switch (image.GetType())
			{
			case ATTACHMENT_IMAGE:
				clearValue.color = {image.GetClearColour().m_r, image.GetClearColour().m_g, image.GetClearColour().m_b, image.GetClearColour().m_a};

				for (auto &subpass : renderpassCreate->GetSubpasses())
				{
					auto subpassBindings = subpass.GetAttachmentBindings();

					if (std::find(subpassBindings.begin(), subpassBindings.end(), image.GetBinding()) != subpassBindings.end())
					{
						m_subpassAttachmentCount[subpass.GetBinding()]++;
					}
				}

				break;
			case ATTACHMENT_DEPTH:
				clearValue.depthStencil = {1.0f, 0};
				m_hasDepth = true;
				break;
			case ATTACHMENT_SWAPCHAIN:
				clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};
				m_hasSwapchain = true;
				break;
			}

			m_clearValues.emplace_back(clearValue);
		}
	}

	RenderStage::~RenderStage()
	{
		delete m_renderpassCreate;
		delete m_depthStencil;
		delete m_renderpass;
		delete m_framebuffers;
	}

	void RenderStage::Rebuild(Swapchain *swapchain)
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		auto samples = Display::Get()->GetMsaaSamples();

		if (m_hasDepth)
		{
			delete m_depthStencil;
			m_depthStencil = new DepthStencil(GetWidth(), GetHeight(), samples);
		}

		if (m_renderpass == nullptr)
		{
			m_renderpass = new Renderpass(*m_renderpassCreate, *m_depthStencil, surfaceFormat.format, samples);
		}

		delete m_framebuffers;
		m_framebuffers = new Framebuffers(GetWidth(), GetHeight(), *m_renderpassCreate, *m_renderpass, *swapchain, *m_depthStencil, samples);

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Renderstage '%i' built in %fms\n", m_stageIndex, debugEnd - debugStart);
#endif
	}

	uint32_t RenderStage::GetWidth() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate->GetWidth();
		}

		return Display::Get()->GetWidth();
	}

	uint32_t RenderStage::GetHeight() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate->GetHeight();
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

	VkFramebuffer RenderStage::GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const
	{
		if (activeSwapchainImage > m_framebuffers->GetFramebuffers().size())
		{
			return m_framebuffers->GetFramebuffers().at(0);
		}

		return m_framebuffers->GetFramebuffers().at(activeSwapchainImage);
	}
}
