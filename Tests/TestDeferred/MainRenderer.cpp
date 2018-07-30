#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Meshes/RendererMeshes.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Shadows/RendererShadows.hpp>
#include <Particles/RendererParticles.hpp>
#include <Models/Shapes/ModelSphere.hpp>

namespace test
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
		{
			4096, 4096, // width / height
			{
				Attachment(0, ATTACHMENT_IMAGE, FORMAT_R8_UNORM) // shadows
			}, // images
			{
				SubpassType(0, {0})
			} // subpasses
		};
	RenderpassCreate *RENDERPASS_1_CREATE = new RenderpassCreate
		{
			0, 0, // width / height
			{
				Attachment(0, ATTACHMENT_DEPTH), // depth
				Attachment(1, ATTACHMENT_SWAPCHAIN), // swapchain
				Attachment(2, ATTACHMENT_IMAGE, FORMAT_R8G8B8A8_UNORM), // colours
				Attachment(3, ATTACHMENT_IMAGE, FORMAT_R16G16_UNORM), // normals
				Attachment(4, ATTACHMENT_IMAGE, FORMAT_R8G8B8A8_UNORM) // materials
			}, // images
			{
				SubpassType(0, {0, 2, 3, 4}),
				SubpassType(1, {1}),
				SubpassType(2, {1}),
			} // subpasses
		};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE})
	{
		AddRenderer<RendererShadows>(GraphicsStage(0, 0))->SetEnabled(false);
		AddRenderer<RendererMeshes>(GraphicsStage(1, 0));
		AddRenderer<RendererParticles>(GraphicsStage(1, 0));
		AddRenderer<RendererDeferred>(GraphicsStage(1, 1));
		AddRenderer<FilterFxaa>(GraphicsStage(1, 2));
		AddRenderer<FilterLensflare>(GraphicsStage(1, 2))->SetEnabled(false);
	//	AddRenderer<FilterTiltshift>(GraphicsStage(1, 2));
	//	AddRenderer<FilterGrain>(GraphicsStage(1, 2));
	//	AddRenderer<PipelineGaussian>(GraphicsStage(1, 2));
		AddRenderer<RendererGuis>(GraphicsStage(1, 2));
		AddRenderer<RendererFonts>(GraphicsStage(1, 2));
	}

	MainRenderer::~MainRenderer()
	{
	}

	void MainRenderer::Update()
	{
		RENDERPASS_0_CREATE->SetWidth(Shadows::Get()->GetShadowSize());
		RENDERPASS_0_CREATE->SetHeight(Shadows::Get()->GetShadowSize());
	}
}
