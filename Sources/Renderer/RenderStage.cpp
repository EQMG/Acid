#include "RenderStage.hpp"

namespace fl
{
	RenderStage::RenderStage(const int &stageIndex, RenderpassCreate *renderpassCreate) :
		m_stageIndex(stageIndex),
		m_lastWidth(renderpassCreate->GetWidth()),
		m_lastHeight(renderpassCreate->GetHeight()),
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
		for (auto image : renderpassCreate->GetImages())
		{
			VkClearValue clearValue = {};

			switch (image.GetType())
			{
			case TypeImage:
				clearValue.color = {*image.GetClearColour().m_elements};
				m_imageAttachments++;
				break;
			case TypeDepth:
				clearValue.depthStencil = {1.0f, 0};
				m_hasDepth = true;
				break;
			case TypeSwapchain:
				clearValue.color = {{0.0f, 0.0f, 0.0f, 0.0f}};
				m_hasSwapchain = true;
				break;
			}

			m_clearValues.push_back(clearValue);
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
#if FL_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		const auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		const VkExtent2D extent2D = {GetWidth(), GetHeight()};
		const VkExtent3D extent3D = {GetWidth(), GetHeight(), 1};

		if (m_hasDepth)
		{
			delete m_depthStencil;
			m_depthStencil = new DepthStencil(extent3D);
		}

		if (m_renderpass == nullptr)
		{
			m_renderpass = new Renderpass(*m_renderpassCreate, *m_depthStencil, surfaceFormat.format);
		}

		delete m_framebuffers;
		m_framebuffers = new Framebuffers(*m_renderpassCreate, *m_renderpass, *swapchain, *m_depthStencil, extent2D);

#if FL_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Renderstage '%i' built in %fms\n", m_stageIndex, debugEnd - debugStart);
#endif
	}

	uint32_t RenderStage::GetWidth() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate->GetWidth();
		}

		return static_cast<uint32_t>(Display::Get()->GetWidth());
	}

	uint32_t RenderStage::GetHeight() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate->GetHeight();
		}

		return static_cast<uint32_t>(Display::Get()->GetHeight());
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
		if (activeSwapchainImage > m_framebuffers->GetFramebuffersCount())
		{
			return m_framebuffers->GetFramebuffers().at(0);
		}

		return m_framebuffers->GetFramebuffers().at(activeSwapchainImage);
	}
}
