#include "RenderStage.hpp"

#include "Devices/Window.hpp"
#include "Graphics.hpp"

namespace acid
{
RenderStage::RenderStage(std::vector<Attachment> images, std::vector<SubpassType> subpasses, const Viewport &viewport) :
	m_attachments{std::move(images)},
	m_subpasses{std::move(subpasses)},
	m_viewport{viewport},
	m_subpassAttachmentCount(m_subpasses.size()),
	m_subpassMultisampled(m_subpasses.size())
{
	for (const auto &image : m_attachments)
	{
		VkClearValue clearValue{};

		switch (image.GetType())
		{
		case Attachment::Type::Image:
			clearValue.color = {{ image.GetClearColour().m_r, image.GetClearColour().m_g, image.GetClearColour().m_b, image.GetClearColour().m_a }};

			for (const auto &subpass : m_subpasses)
			{
				if (auto subpassBindings = subpass.GetAttachmentBindings();
					std::find(subpassBindings.begin(), subpassBindings.end(), image.GetBinding()) != subpassBindings.end())
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
	auto lastRenderArea = m_renderArea;

	m_renderArea.SetOffset(m_viewport.GetOffset());

	if (m_viewport.GetSize())
	{
		m_renderArea.SetExtent(m_viewport.GetScale() * *m_viewport.GetSize());
	}
	else
	{
		m_renderArea.SetExtent(m_viewport.GetScale() * Window::Get()->GetSize());
	}

	m_renderArea.SetAspectRatio(static_cast<float>(m_renderArea.GetExtent().m_x) / static_cast<float>(m_renderArea.GetExtent().m_y));
	m_renderArea.SetExtent(m_renderArea.GetExtent() + m_renderArea.GetOffset());

	m_outOfDate = m_renderArea != lastRenderArea;
}

void RenderStage::Rebuild(const Swapchain &swapchain)
{
#if defined(ACID_VERBOSE)
	auto debugStart = Time::Now();
#endif

	Update();

	auto physicalDevice = Graphics::Get()->GetPhysicalDevice();
	auto surface = Graphics::Get()->GetSurface();

	auto msaaSamples = physicalDevice->GetMsaaSamples();

	if (m_depthAttachment)
	{
		m_depthStencil = std::make_unique<ImageDepth>(m_renderArea.GetExtent(), m_depthAttachment->IsMultisampled() ? msaaSamples : VK_SAMPLE_COUNT_1_BIT);
	}

	if (!m_renderpass)
	{
		m_renderpass = std::make_unique<Renderpass>(*this, m_depthStencil->GetFormat(), surface->GetFormat().format, msaaSamples);
	}

	m_framebuffers = std::make_unique<Framebuffers>(m_renderArea.GetExtent(), *this, *m_renderpass, swapchain, *m_depthStencil, msaaSamples);
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
	Log::Out("Render Stage created in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

std::optional<Attachment> RenderStage::GetAttachment(const std::string &name) const
{
	auto it{std::find_if(m_attachments.begin(), m_attachments.end(), [name](const Attachment &a)
	{
		return a.GetName() == name;
	})};

	if (it == m_attachments.end())
	{
		return std::nullopt;
	}

	return *it;
}

std::optional<Attachment> RenderStage::GetAttachment(uint32_t binding) const
{
	auto it{std::find_if(m_attachments.begin(), m_attachments.end(), [binding](const Attachment &a)
	{
		return a.GetBinding() == binding;
	})};

	if (it == m_attachments.end())
	{
		return std::nullopt;
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

const VkFramebuffer &RenderStage::GetActiveFramebuffer(uint32_t activeSwapchainImage) const
{
	if (activeSwapchainImage > m_framebuffers->GetFramebuffers().size())
	{
		return m_framebuffers->GetFramebuffers().at(0);
	}

	return m_framebuffers->GetFramebuffers().at(activeSwapchainImage);
}
}
