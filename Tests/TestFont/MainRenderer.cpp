#include "MainRenderer.hpp"

#include <Fonts/RenderFonts.hpp>
#include <Fonts/RenderFonts2.hpp>
#include <Guis/RenderGuis.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
MainRenderer::MainRenderer()
{
	std::vector<std::unique_ptr<RenderStage>> renderStages;

	std::vector<Attachment> renderpassAttachments0 = { Attachment(0, "depth", Attachment::Type::Depth),
		Attachment(1, "swapchain", Attachment::Type::Swapchain, false, VK_FORMAT_R8G8B8A8_UNORM, Colour::White) };
	std::vector<SubpassType> renderpassSubpasses0 = { SubpassType(0, { 0, 1 }) };
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0));
	Renderer::Get()->SetRenderStages(std::move(renderStages));
	
	auto &renderHolder = GetRenderHolder();
	renderHolder.Add<RenderGuis>(Pipeline::Stage(0, 0));
	renderHolder.Add<RenderFonts>(Pipeline::Stage(0, 0));
	renderHolder.Add<RenderFonts2>(Pipeline::Stage(0, 0));
}

void MainRenderer::Update()
{
}
}
