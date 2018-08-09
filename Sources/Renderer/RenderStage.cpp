#include "RenderStage.hpp"

#include "Display/Display.hpp"

namespace acid
{
	RenderStage::RenderStage(const int &stageIndex, RenderpassCreate *renderpassCreate) :
		m_lastWidth(renderpassCreate->GetWidth()),
		m_lastHeight(renderpassCreate->GetHeight()),
		m_stageIndex(stageIndex),
		m_renderpassCreate(renderpassCreate),
		m_depthStencil(nullptr),
		m_renderpass(nullptr),
		m_framebuffers(nullptr),
		m_clearValues(std::vector<VkClearValue>()),
		m_imageAttachments(0),
		m_hasDepth(false),
		m_hasSwapchain(false),
		m_fitDisplaySize(m_renderpassCreate->GetHeight() == 0)
	{
		for (auto &image : renderpassCreate->GetImages())
		{
			VkClearValue clearValue = {};

			switch (image.GetType())
			{
			case ATTACHMENT_IMAGE:
				clearValue.color = {{image.GetClearColour().m_r, image.GetClearColour().m_g, image.GetClearColour().m_b, image.GetClearColour().m_a}};
				m_imageAttachments++;
				break;
			case ATTACHMENT_DEPTH:
				clearValue.depthStencil = {1.0f, 0};
				m_hasDepth = true;
				break;
			case ATTACHMENT_SWAPCHAIN:
				clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
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

		VkExtent2D extent2D = {GetWidth(), GetHeight()};
		VkExtent3D extent3D = {GetWidth(), GetHeight(), 1};

		if (m_hasDepth)
		{
			delete m_depthStencil;
			m_depthStencil = new DepthStencil(extent3D.width, extent3D.height, samples);
		}

		if (m_renderpass == nullptr)
		{
			m_renderpass = new Renderpass(*m_renderpassCreate, *m_depthStencil, surfaceFormat.format, samples);
		}

		delete m_framebuffers;
		m_framebuffers = new Framebuffers(*m_renderpassCreate, *m_renderpass, *swapchain, *m_depthStencil, extent2D, samples);

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
