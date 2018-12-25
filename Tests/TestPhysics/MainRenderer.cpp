#include "MainRenderer.hpp"

#include <Fonts/RendererFonts.hpp>
#include <Gizmos/RendererGizmos.hpp>
#include <Guis/RendererGuis.hpp>
#include <Inputs/Keyboard.hpp>
#include <Meshes/RendererMeshes.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/RendererParticles.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterCrt.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Post/Filters/FilterDof.hpp>
#include <Post/Filters/FilterEmboss.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Post/Filters/FilterPixel.hpp>
#include <Post/Filters/FilterSsao.hpp>
#include <Post/Filters/FilterTiltshift.hpp>
#include <Post/Filters/FilterTone.hpp>
#include <Post/Filters/FilterVignette.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/RendererShadows.hpp>

namespace test
{
	MainRenderer::MainRenderer()
	{
		std::vector<Attachment> renderpassImages0 = {
			Attachment(0, "shadows", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8_UNORM)
		};
		std::vector<SubpassType> renderpassSubpasses0 = {
			SubpassType(0, {0})
		};
		m_renderpassCreates.emplace_back(RenderpassCreate(renderpassImages0, renderpassSubpasses0, 4096, 4096));

		std::vector<Attachment> renderpassImages1 = {
			Attachment(0, "depth", ATTACHMENT_TYPE_DEPTH, VK_FORMAT_D32_SFLOAT_S8_UINT, false),
			Attachment(1, "swapchain", ATTACHMENT_TYPE_SWAPCHAIN),
			Attachment(2, "diffuse", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false),
			Attachment(3, "normals", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT, false),
			Attachment(4, "materials", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false),
			Attachment(5, "resolved", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM)
		};
		std::vector<SubpassType> renderpassSubpasses1 = {
			SubpassType(0, {0, 2, 3, 4}),
			SubpassType(1, {0, 5}),
			SubpassType(2, {0, 1})
		};
		m_renderpassCreates.emplace_back(RenderpassCreate(renderpassImages1, renderpassSubpasses1));
	}

	void MainRenderer::Start()
	{
		auto &rendererRegister = Renderer::Get()->GetRendererRegister();

	//	rendererRegister.Add<RendererShadows>(GraphicsStage(0, 0));

		rendererRegister.Add<RendererMeshes>(GraphicsStage(1, 0));

		rendererRegister.Add<RendererDeferred>(GraphicsStage(1, 1), DEFERRED_SIMPLE);
		rendererRegister.Add<RendererParticles>(GraphicsStage(1, 1));

		rendererRegister.Add<FilterFxaa>(GraphicsStage(1, 2));
	//	rendererRegister.Add<FilterTone>(GraphicsStage(1, 2));
	//	rendererRegister.Add<FilterSsao>(GraphicsStage(1, 2));
	//	rendererRegister.Add()->AddRenderer<PipelineBlur>(GraphicsStage(1, 2), 1.8f, BLUR_TYPE_5, false, 0.6f, 1.0f);
	//	rendererRegister.Add<FilterDof>(GraphicsStage(1, 2), sceneBlur, 1.11f);
	//	rendererRegister.Add<FilterEmboss>(GraphicsStage(1, 2));
	//	rendererRegister.Add<FilterCrt>(GraphicsStage(1, 2));
	//	rendererRegister.Add<FilterLensflare>(GraphicsStage(1, 2));
	//	rendererRegister.Add<FilterTiltshift>(GraphicsStage(1, 2));
	//	rendererRegister.Add<FilterPixel>(GraphicsStage(1, 2), 8.0f);
	//	rendererRegister.Add<FilterVignette>(GraphicsStage(1, 2));
	//	rendererRegister.Add<FilterGrain>(GraphicsStage(1, 2));
		rendererRegister.Add<FilterDefault>(GraphicsStage(1, 2), true);
	//	rendererRegister.Add<RendererGizmos>(GraphicsStage(1, 2));
		rendererRegister.Add<RendererGuis>(GraphicsStage(1, 2));
		rendererRegister.Add<RendererFonts>(GraphicsStage(1, 2));
	}

	void MainRenderer::Update()
	{
		auto &renderpassCreate0 = Renderer::Get()->GetRenderStage(0)->GetRenderpassCreate();
		renderpassCreate0.SetWidth(Shadows::Get()->GetShadowSize());
		renderpassCreate0.SetHeight(Shadows::Get()->GetShadowSize()); // * RendererShadows::NUM_CASCADES

	//	auto &renderpassCreate1 = Renderer::Get()->GetRenderStage(1)->GetRenderpassCreate();
	//	renderpassCreate1.SetScale(0.75f);

	//	Renderer::Get()->GetRenderer<FilterVignette>(true)->SetEnabled(Keyboard::Get()->GetKey(KEY_I));
	}
}
