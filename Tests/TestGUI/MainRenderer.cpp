#include "MainRenderer.hpp"

#include <Fonts/SubrenderFonts.hpp>
#include <Fonts/SubrenderFonts2.hpp>
#include <Guis/SubrenderGuis.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
MainRenderer::MainRenderer()
{
	std::vector<std::unique_ptr<RenderStage>> renderStages;

	std::vector<Attachment> renderpassAttachments0 = {
		{0, "depth", Attachment::Type::Depth},
		{1, "swapchain", Attachment::Type::Swapchain}
	};
	std::vector<SubpassType> renderpassSubpasses0 = {
		{0, {0, 1}}
	};
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0));
	Graphics::Get()->SetRenderStages(std::move(renderStages));

	Graphics::Get()->ClearSubrenders();
	Graphics::Get()->AddSubrender<SubrenderGuis>({0, 0});
	Graphics::Get()->AddSubrender<SubrenderFonts>({0, 0});
	Graphics::Get()->AddSubrender<SubrenderFonts2>({0, 0});
}

void MainRenderer::Update()
{
	//auto renderpassCreate0 = Renderer::Get()->GetRenderStage(0);
	//renderpassCreate0->GetViewport().SetScale(0.8f);
}
}
