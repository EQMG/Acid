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
#include <Post/Filters/FilterSsao.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Post/Filters/FilterVignette.hpp>
#include <Inputs/Keyboard.hpp>

namespace test
{
	const RenderpassCreate RENDERPASS_0_CREATE = RenderpassCreate
	{
		4096, 4096, // width, height
		{
			Attachment(0, ATTACHMENT_IMAGE, VK_FORMAT_R8_UNORM) // shadows
		}, // images
		{
			SubpassType(0, {0})
		} // subpasses
	};
	const RenderpassCreate RENDERPASS_1_CREATE = RenderpassCreate
	{
		0, 0, // width, height
		{
			Attachment(0, ATTACHMENT_DEPTH, VK_FORMAT_D32_SFLOAT_S8_UINT, false), // depth
			Attachment(1, ATTACHMENT_SWAPCHAIN), // swapchain
			Attachment(2, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false), // diffuse
			Attachment(3, ATTACHMENT_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT, false), // normals
			Attachment(4, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false), // materials
			Attachment(5, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM) // resolved
		}, // images
		{
			SubpassType(0, {0, 2, 3, 4}),
			SubpassType(1, {0, 5}),
			SubpassType(2, {1})
		} // subpasses
	};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE})
	{
	}

	void MainRenderer::Start()
	{
	//	Renderer::Get()->AddRenderer<RendererShadows>(GraphicsStage(0, 0));

		Renderer::Get()->AddRenderer<RendererMeshes>(GraphicsStage(1, 0));

		Renderer::Get()->AddRenderer<RendererDeferred>(GraphicsStage(1, 1));
		Renderer::Get()->AddRenderer<RendererParticles>(GraphicsStage(1, 1));

		Renderer::Get()->AddRenderer<FilterDefault>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterFxaa>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterLensflare>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterTiltshift>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterGrain>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterVignette>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<FilterSsao>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<PipelineGaussian>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<RendererGuis>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<RendererFonts>(GraphicsStage(1, 2));
	}

	void MainRenderer::Update()
	{
		auto &renderpassCreate0 = Renderer::Get()->GetRenderStage(0)->GetRenderpassCreate();
		renderpassCreate0.SetWidth(Shadows::Get()->GetShadowSize());
		renderpassCreate0.SetHeight(Shadows::Get()->GetShadowSize());

	//	Renderer::Get()->GetRenderer<FilterVignette>(true)->SetEnabled(Keyboard::Get()->GetKey(KEY_I));
	}
}
