#include "RenderStage.hpp"

#include "Devices/Window.hpp"
#include "Graphics.hpp"

namespace acid {
RenderStage::RenderStage(std::vector<Attachment> images, std::vector<SubpassType> subpasses, const Viewport &viewport) :
	attachments(std::move(images)),
	subpasses(std::move(subpasses)),
	viewport(viewport),
	subpassAttachmentCount(this->subpasses.size()),
	subpassMultisampled(this->subpasses.size()) {
	for (const auto &image : attachments) {
		VkClearValue clearValue = {};

		switch (image.GetType()) {
		case Attachment::Type::Image:
			clearValue.color = {{image.GetClearColour().r, image.GetClearColour().g, image.GetClearColour().b, image.GetClearColour().a}};

			for (const auto &subpass : this->subpasses) {
				if (auto subpassBindings = subpass.GetAttachmentBindings();
					std::find(subpassBindings.begin(), subpassBindings.end(), image.GetBinding()) != subpassBindings.end()) {
					subpassAttachmentCount[subpass.GetBinding()]++;

					if (image.IsMultisampled()) {
						subpassMultisampled[subpass.GetBinding()] = true;
					}
				}
			}

			break;
		case Attachment::Type::Depth:
			clearValue.depthStencil = {1.0f, 0};
			depthAttachment = image;
			break;
		case Attachment::Type::Swapchain:
			clearValue.color = {{image.GetClearColour().r, image.GetClearColour().g, image.GetClearColour().b, image.GetClearColour().a}};
			swapchainAttachment = image;
			break;
		}

		clearValues.emplace_back(clearValue);
	}
}

void RenderStage::Update() {
	auto lastRenderArea = renderArea;

	renderArea.SetOffset(viewport.GetOffset());

	if (viewport.GetSize())
		renderArea.SetExtent(viewport.GetScale() * *viewport.GetSize());
	else
		renderArea.SetExtent(viewport.GetScale() * Window::Get()->GetSize());

	renderArea.SetAspectRatio(static_cast<float>(renderArea.GetExtent().x) / static_cast<float>(renderArea.GetExtent().y));
	renderArea.SetExtent(renderArea.GetExtent() + renderArea.GetOffset());

	outOfDate = renderArea != lastRenderArea;
}

void RenderStage::Rebuild(const Swapchain &swapchain) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	Update();

	auto physicalDevice = Graphics::Get()->GetPhysicalDevice();
	auto surface = Graphics::Get()->GetSurface();

	auto msaaSamples = physicalDevice->GetMsaaSamples();

	if (depthAttachment) {
		depthStencil = std::make_unique<ImageDepth>(renderArea.GetExtent(), depthAttachment->IsMultisampled() ? msaaSamples : VK_SAMPLE_COUNT_1_BIT);
	}

	if (!renderpass) {
		renderpass = std::make_unique<Renderpass>(*this, depthStencil ? depthStencil->GetFormat() : VK_FORMAT_UNDEFINED, surface->GetFormat().format, msaaSamples);
	}

	framebuffers = std::make_unique<Framebuffers>(renderArea.GetExtent(), *this, *renderpass, swapchain, *depthStencil, msaaSamples);
	outOfDate = false;

	descriptors.clear();

	for (const auto &image : attachments) {
		if (image.GetType() == Attachment::Type::Depth) {
			descriptors.emplace(image.GetName(), depthStencil.get());
		} else {
			descriptors.emplace(image.GetName(), framebuffers->GetAttachment(image.GetBinding()));
		}
	}

#if defined(ACID_DEBUG)
	Log::Out("Render Stage created in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

std::optional<Attachment> RenderStage::GetAttachment(const std::string &name) const {
	auto it = std::find_if(attachments.begin(), attachments.end(), [name](const Attachment &a) {
		return a.GetName() == name;
	});

	if (it == attachments.end())
		return std::nullopt;
	return *it;
}

std::optional<Attachment> RenderStage::GetAttachment(uint32_t binding) const {
	auto it = std::find_if(attachments.begin(), attachments.end(), [binding](const Attachment &a) {
		return a.GetBinding() == binding;
	});

	if (it == attachments.end())
		return std::nullopt;
	return *it;
}

const Descriptor *RenderStage::GetDescriptor(const std::string &name) const {
	auto it = descriptors.find(name);

	if (it != descriptors.end())
		return it->second;
	return nullptr;
}

const VkFramebuffer &RenderStage::GetActiveFramebuffer(uint32_t activeSwapchainImage) const {
	if (activeSwapchainImage > framebuffers->GetFramebuffers().size())
		return framebuffers->GetFramebuffers().at(0);

	return framebuffers->GetFramebuffers().at(activeSwapchainImage);
}
}
