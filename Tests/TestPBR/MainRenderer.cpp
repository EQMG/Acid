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
#include <Post/Filters/FilterDefault.hpp>
#include <Inputs/Keyboard.hpp>

namespace test
{
	const RenderpassCreate RENDERPASS_0_CREATE = RenderpassCreate
	{
		{
			Attachment(0, "shadows", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8_UNORM)
		}, // images
		{
			SubpassType(0, {0})
		}, // subpasses
		4096, 4096 // width, height
	};
	const RenderpassCreate RENDERPASS_1_CREATE = RenderpassCreate
	{
		{
			Attachment(0, "depth", ATTACHMENT_TYPE_DEPTH, VK_FORMAT_D32_SFLOAT_S8_UINT, false),
			Attachment(1, "swapchain", ATTACHMENT_TYPE_SWAPCHAIN),
			Attachment(2, "diffuse", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false),
			Attachment(3, "normals", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT, false),
			Attachment(4, "materials", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false),
			Attachment(5, "resolved", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM)
		}, // images
		{
			SubpassType(0, {0, 2, 3, 4}),
			SubpassType(1, {0, 5}),
			SubpassType(2, {1})
		} // subpasses
	};

	MainRenderer::MainRenderer() :
		RenderManager({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE})
	{
	}

	void MainRenderer::Start()
	{
		auto &rendererRegister = Renderer::Get()->GetRendererRegister();
	//	rendererRegister.Add<RendererShadows>(GraphicsStage(0, 0));

		rendererRegister.Add<RendererMeshes>(GraphicsStage(1, 0));

		rendererRegister.Add<RendererDeferred>(GraphicsStage(1, 1), DEFERRED_IBL);
		rendererRegister.Add<RendererParticles>(GraphicsStage(1, 1));

		rendererRegister.Add<FilterDefault>(GraphicsStage(1, 2), true);
		rendererRegister.Add<RendererGuis>(GraphicsStage(1, 2));
		rendererRegister.Add<RendererFonts>(GraphicsStage(1, 2));
	}

	void MainRenderer::Update()
	{
		auto &renderpassCreate0 = Renderer::Get()->GetRenderStage(0)->GetRenderpassCreate();
		renderpassCreate0.SetWidth(Shadows::Get()->GetShadowSize());
		renderpassCreate0.SetHeight(Shadows::Get()->GetShadowSize());
	}
}
