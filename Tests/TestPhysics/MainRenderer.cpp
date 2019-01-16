#include "MainRenderer.hpp"

#include <Fonts/RendererFonts.hpp>
#include <Gizmos/RendererGizmos.hpp>
#include <Guis/RendererGuis.hpp>
#include <Devices/Keyboard.hpp>
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
			Attachment(0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM)
		};
		std::vector<SubpassType> renderpassSubpasses0 = {
			SubpassType(0, {0})
		};
		m_renderpassCreates.emplace_back(RenderpassCreate(renderpassImages0, renderpassSubpasses0, 4096, 4096));

		std::vector<Attachment> renderpassImages1 = {
			Attachment(0, "depth", Attachment::Type::Depth, false),
			Attachment(1, "swapchain", Attachment::Type::Swapchain),
			Attachment(2, "diffuse", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM),
			Attachment(3, "normals", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
			Attachment(4, "materials", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM),
			Attachment(5, "resolved", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM)
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
		auto &rendererContainer = GetRendererContainer();

	//	rendererContainer.Add<RendererShadows>(GraphicsStage(0, 0));

		rendererContainer.Add<RendererMeshes>(GraphicsStage(1, 0));

		rendererContainer.Add<RendererDeferred>(GraphicsStage(1, 1), RendererDeferred::Type::Simple);
	//	rendererContainer.Add<RendererParticles>(GraphicsStage(1, 1));

	//	rendererContainer.Add<FilterFxaa>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterTone>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterSsao>(GraphicsStage(1, 2));
	//	rendererContainer.Add()->AddRenderer<PipelineBlur>(GraphicsStage(1, 2), 1.8f, PipelineBlur::Type::_5, false, 0.6f, 1.0f);
	//	rendererContainer.Add<FilterDof>(GraphicsStage(1, 2), sceneBlur, 1.11f);
	//	rendererContainer.Add<FilterEmboss>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterCrt>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterLensflare>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterTiltshift>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterPixel>(GraphicsStage(1, 2), 8.0f);
	//	rendererContainer.Add<FilterVignette>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterGrain>(GraphicsStage(1, 2));
		rendererContainer.Add<FilterDefault>(GraphicsStage(1, 2), true);
	//	rendererContainer.Add<RendererGizmos>(GraphicsStage(1, 2));
		rendererContainer.Add<RendererGuis>(GraphicsStage(1, 2));
		rendererContainer.Add<RendererFonts>(GraphicsStage(1, 2));
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
