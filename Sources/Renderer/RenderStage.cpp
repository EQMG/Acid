#include "RenderStage.hpp"

#include "Renderer.hpp"

namespace acid
{
RenderStage::RenderStage(std::vector<Attachment> images, std::vector<SubpassType> subpasses, const Viewport &viewport) :
	m_attachments(std::move(images)),
	m_subpasses(std::move(subpasses)),
	m_viewport(viewport),
	m_renderpass(nullptr),
	m_depthStencil(nullptr),
	m_framebuffers(nullptr),
	m_subpassAttachmentCount(m_subpasses.size()),
	m_depthAttachment({}),
	m_swapchainAttachment({}),
	m_subpassMultisampled(m_subpasses.size()),
	m_width(0),
	m_height(0),
	m_aspectRatio(1.0f),
	m_outOfDate(false)
{
	for (const auto &image : m_attachments)
	{
		VkClearValue clearValue = {};

		switch (image.GetType())
		{
		case Attachment::Type::Image:
			clearValue.color = {{ image.GetClearColour().m_r, image.GetClearColour().m_g, image.GetClearColour().m_b, image.GetClearColour().m_a }};

			for (const auto &subpass : m_subpasses)
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
		case Attachment::Type::Depth:
			clearValue.depthStencil = { 1.0f, 0 };
			m_depthAttachment = image;
			break;
		case Attachment::Type::Swapchain:
			clearValue.color = {{ image.GetClearColour().m_r, image.GetClearColour().m_g, image.GetClearColour().m_b, image.GetClearColour().m_a }};
			m_swapchainAttachment = image;
			break;
		}

		m_clearValues.emplace_back(clearValue);
	}
}

void RenderStage::Update()
{
	uint32_t lastWidth = m_width;
	uint32_t lastHeight = m_height;

	if (m_viewport.GetWidth())
	{
		m_width = *m_viewport.GetWidth();
	}
	else
	{
		m_width = Window::Get()->GetWidth();
	}

	if (m_viewport.GetHeight())
	{
		m_height = *m_viewport.GetHeight();
	}
	else
	{
		m_height = Window::Get()->GetHeight();
	}

	// TODO: Use viewport offset, fix scale.
	m_width = static_cast<uint32_t>(m_viewport.GetScale().m_x * static_cast<float>(m_width));
	m_height = static_cast<uint32_t>(m_viewport.GetScale().m_y * static_cast<float>(m_height));
	m_aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);

	m_outOfDate = m_width != lastWidth || m_height != lastHeight;
}

void RenderStage::Rebuild(const Swapchain &swapchain)
{
#if defined(ACID_VERBOSE)
	auto debugStart = Engine::GetTime();
#endif

	Update();

	auto physicalDevice = Renderer::Get()->GetPhysicalDevice();
	auto surface = Renderer::Get()->GetSurface();

	auto msaaSamples = physicalDevice->GetMsaaSamples();

	if (m_depthAttachment)
	{
		m_depthStencil = std::make_unique<ImageDepth>(m_width, m_height, m_depthAttachment->IsMultisampled() ? msaaSamples : VK_SAMPLE_COUNT_1_BIT);
	}

	if (m_renderpass == nullptr)
	{
		m_renderpass = std::make_unique<Renderpass>(*this, m_depthStencil->GetFormat(), surface->GetFormat().format, msaaSamples);
	}

	m_framebuffers = std::make_unique<Framebuffers>(m_width, m_height, *this, *m_renderpass, swapchain, *m_depthStencil, msaaSamples);
	m_outOfDate = false;

	m_descriptors.clear();

	for (const auto &image : m_attachments)
	{
		if (image.GetType() == Attachment::Type::Depth)
		{
			m_descriptors.emplace(image.GetName(), m_depthStencil.get());
		}
		else
		{
			m_descriptors.emplace(image.GetName(), m_framebuffers->GetAttachment(image.GetBinding()));
		}
	}

#if defined(ACID_VERBOSE)
	auto debugEnd = Engine::GetTime();
	Log::Out("Render Stage built in %.3fms\n", (debugEnd - debugStart).AsMilliseconds<float>());
#endif
}

std::optional<Attachment> RenderStage::GetAttachment(const std::string &name) const
{
	auto it = std::find_if(m_attachments.begin(), m_attachments.end(), [name](const Attachment &a)
	{
		return a.GetName() == name;
	});

	if (it == m_attachments.end())
	{
		return {};
	}

	return *it;
}

std::optional<Attachment> RenderStage::GetAttachment(const uint32_t &binding) const
{
	auto it = std::find_if(m_attachments.begin(), m_attachments.end(), [binding](const Attachment &a)
	{
		return a.GetBinding() == binding;
	});

	if (it == m_attachments.end())
	{
		return {};
	}

	return *it;
}

const Descriptor *RenderStage::GetDescriptor(const std::string &name) const
{
	auto it = m_descriptors.find(name);

	if (it != m_descriptors.end())
	{
		return it->second;
	}

	return nullptr;
}

const VkFramebuffer &RenderStage::GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const
{
	if (activeSwapchainImage > m_framebuffers->GetFramebuffers().size())
	{
		return m_framebuffers->GetFramebuffers().at(0);
	}

	return m_framebuffers->GetFramebuffers().at(activeSwapchainImage);
}
}
