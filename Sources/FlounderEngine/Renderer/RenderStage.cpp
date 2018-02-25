#include "RenderStage.hpp"

namespace Flounder
{
	RenderStage::RenderStage(RenderpassCreate *renderpassCreate) :
		m_renderpassCreate(renderpassCreate),
		m_depthStencil(nullptr),
		m_renderpass(nullptr),
		m_framebuffers(nullptr),
		m_clearValues(std::vector<VkClearValue>()),
		m_imageAttachments(0),
		m_hasDepth(false),
		m_hasSwapchain(false),
		m_fitDisplaySize(m_renderpassCreate->m_height == 0)
	{
		for (auto image : renderpassCreate->images)
		{
			VkClearValue clearValue = {};

			switch (image.m_type)
			{
			case TypeImage:
				clearValue.color = {*image.m_clearColour.m_elements};
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
		const auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		const VkExtent2D extent2D = { GetWidth(), GetHeight() };
		const VkExtent3D extent3D = { GetWidth(), GetHeight(), 1 };

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
	}

	uint32_t RenderStage::GetWidth() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate->m_width;
		}

		return static_cast<uint32_t>(Display::Get()->GetWidth());
	}

	uint32_t RenderStage::GetHeight() const
	{
		if (!m_fitDisplaySize)
		{
			return m_renderpassCreate->m_height;
		}

		return static_cast<uint32_t>(Display::Get()->GetHeight());
	}

	bool RenderStage::IsOutOfDate(const VkExtent2D &extent2D) const
	{
		return m_fitDisplaySize ? GetWidth() != extent2D.width || GetHeight() != extent2D.height : false;
	}

	VkFramebuffer RenderStage::GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const
	{
		if (activeSwapchainImage > m_framebuffers->GetFramebuffersCount())
		{
			return m_framebuffers->GetFramebuffers()[0];
		}

		return m_framebuffers->GetFramebuffers()[activeSwapchainImage];
	}
}
