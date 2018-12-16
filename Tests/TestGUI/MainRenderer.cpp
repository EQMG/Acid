#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Guis/RendererGuis.hpp>
#include <Fonts/RendererFonts.hpp>

namespace test
{
	const RenderpassCreate RENDERPASS_0_CREATE = RenderpassCreate
	{
		{
			Attachment(0, "depth", ATTACHMENT_TYPE_DEPTH),
			Attachment(1, "swapchain", ATTACHMENT_TYPE_SWAPCHAIN)
		}, // images
		{
			SubpassType(0, {0, 1}),
		} // subpasses
	};

	MainRenderer::MainRenderer() :
		RenderManager({RENDERPASS_0_CREATE})
	{
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
