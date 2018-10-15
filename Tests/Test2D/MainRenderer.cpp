#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Meshes/RendererMeshes.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Post/Filters/FilterVignette.hpp>

namespace test
{
	const RenderpassCreate RENDERPASS_0_CREATE = RenderpassCreate
	{
		{
			Attachment(0, "depth", ATTACHMENT_DEPTH, VK_FORMAT_D32_SFLOAT_S8_UINT, false),
			Attachment(1, "swapchain", ATTACHMENT_SWAPCHAIN),
			Attachment(2, "resolved", ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false, Colour::WHITE)
		}, // images
		{
			SubpassType(0, {0, 2}),
			SubpassType(1, {1})
		} // subpasses
	};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE})
	{
	}

	void MainRenderer::Start()
	{
		Renderer::Get()->AddRenderer<RendererMeshes>(GraphicsStage(0, 0));

		Renderer::Get()->AddRenderer<FilterDefault>(GraphicsStage(0, 1));
		Renderer::Get()->AddRenderer<FilterGrain>(GraphicsStage(0, 1));
		Renderer::Get()->AddRenderer<FilterVignette>(GraphicsStage(0, 1));

		Renderer::Get()->AddRenderer<RendererGuis>(GraphicsStage(0, 1));
		Renderer::Get()->AddRenderer<RendererFonts>(GraphicsStage(0, 1));
	}

	void MainRenderer::Update()
	{
	}
}
