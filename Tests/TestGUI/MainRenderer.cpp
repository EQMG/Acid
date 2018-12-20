#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Guis/RendererGuis.hpp>
#include <Fonts/RendererFonts.hpp>

namespace test
{
	MainRenderer::MainRenderer()
	{
		std::vector<Attachment> renderpassImages0 = {
			Attachment(0, "depth", ATTACHMENT_TYPE_DEPTH),
			Attachment(1, "swapchain", ATTACHMENT_TYPE_SWAPCHAIN)
		};
		std::vector<SubpassType> renderpassSubpasses0 = {
			SubpassType(0, {0, 1})
		};
		m_renderpassCreates.emplace_back(RenderpassCreate(renderpassImages0, renderpassSubpasses0));
	}

	void MainRenderer::Start()
	{
		auto &rendererRegister = Renderer::Get()->GetRendererRegister();
		rendererRegister.Add<RendererGuis>(GraphicsStage(0, 0));
		rendererRegister.Add<RendererFonts>(GraphicsStage(0, 0));
	}

	void MainRenderer::Update()
	{
	//	auto &renderpassCreate0 = Renderer::Get()->GetRenderStage(0)->GetRenderpassCreate();
	//	renderpassCreate0.SetScale(0.8f);
	}
}
