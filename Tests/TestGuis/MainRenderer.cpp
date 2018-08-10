#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Guis/RendererGuis.hpp>
#include <Fonts/RendererFonts.hpp>

namespace test
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
		{
			0, 0, // width / height
			{
				Attachment(0, ATTACHMENT_DEPTH), // depth
				Attachment(1, ATTACHMENT_SWAPCHAIN) // swapchain
			}, // images
			{
				SubpassType(0, {1}),
			} // subpasses
		};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE})
	{
		AddRenderer<RendererGuis>(GraphicsStage(0, 0));
		AddRenderer<RendererFonts>(GraphicsStage(0, 0));
	}

	MainRenderer::~MainRenderer()
	{
	}

	void MainRenderer::Update()
	{
	}
}
