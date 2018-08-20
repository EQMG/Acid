#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Meshes/RendererMeshes.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>
#include <Shadows/RendererShadows.hpp>
#include <Particles/RendererParticles.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Post/Filters/FilterTiltshift.hpp>
#include <Post/Filters/FilterDefault.hpp>

namespace test
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
	{
		4096, 4096, // width, height
		{
			Attachment(0, ATTACHMENT_IMAGE, VK_FORMAT_R8_UNORM) // shadows
		}, // images
		{
			SubpassType(0, {0})
		} // subpasses
	};
	RenderpassCreate *RENDERPASS_1_CREATE = new RenderpassCreate
	{
		0, 0, // width, height
		{
			Attachment(0, ATTACHMENT_DEPTH), // depth
			Attachment(1, ATTACHMENT_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT), // positions (world-space)
			Attachment(2, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM), // diffuse
			Attachment(3, ATTACHMENT_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT), // normals (world-space)
			Attachment(4, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM), // materials
		}, // images
		{
			SubpassType(0, {0, 1, 2, 3, 4})
		} // subpasses
	};
	RenderpassCreate *RENDERPASS_2_CREATE = new RenderpassCreate
	{
		0, 0, // width, height
		{
			Attachment(0, ATTACHMENT_SWAPCHAIN), // swapchain
			Attachment(1, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM) // colour
		}, // images
		{
			SubpassType(0, {1}),
			SubpassType(1, {0})
		} // subpasses
	};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE, RENDERPASS_2_CREATE})
	{
	//	AddRenderer<RendererShadows>(GraphicsStage(0, 0));

		AddRenderer<RendererMeshes>(GraphicsStage(1, 0));
	//	AddRenderer<RendererParticles>(GraphicsStage(1, 0));

		AddRenderer<RendererDeferred>(GraphicsStage(2, 0));
		AddRenderer<FilterDefault>(GraphicsStage(2, 1));
	//	AddRenderer<FilterFxaa>(GraphicsStage(2, 1));
	//	AddRenderer<FilterLensflare>(GraphicsStage(2, 1));
	//	AddRenderer<FilterTiltshift>(GraphicsStage(2, 1));
	//	AddRenderer<FilterGrain>(GraphicsStage(2, 1));
	//	AddRenderer<PipelineGaussian>(GraphicsStage(2, 1));
		AddRenderer<RendererGuis>(GraphicsStage(2, 1));
		AddRenderer<RendererFonts>(GraphicsStage(2, 1));
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
