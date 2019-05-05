#include "MainRenderer.hpp"

#include <Fonts/RendererFonts.hpp>
#include <Gizmos/RendererGizmos.hpp>
#include <Guis/RendererGuis.hpp>
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
#include <Post/Pipelines/PipelineBlur.hpp>
#include <Renderer/Renderer.hpp>
#include <Shadows/RendererShadows.hpp>
#include "Devices/Keyboard.hpp"

namespace test
{
MainRenderer::MainRenderer()
{
	std::vector<std::unique_ptr<RenderStage>> renderStages;

	std::vector<Attachment> renderpassAttachments0 = { Attachment(0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R16_SFLOAT) };
	std::vector<SubpassType> renderpassSubpasses0 = { SubpassType(0, { 0 }) };
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0, Viewport(Vector2ui(4096, 4096))));

	std::vector<Attachment> renderpassAttachments1 = { Attachment(0, "depth", Attachment::Type::Depth, false), Attachment(1, "swapchain", Attachment::Type::Swapchain),
		Attachment(2, "position", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
		Attachment(3, "diffuse", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM), Attachment(4, "normal", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
		Attachment(5, "material", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM), Attachment(6, "resolved", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM) };
	std::vector<SubpassType> renderpassSubpasses1 = { SubpassType(0, { 0, 2, 3, 4, 5 }), SubpassType(1, { 0, 6 }), SubpassType(2, { 0, 1 }) };
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
	Renderer::Get()->SetRenderStages(std::move(renderStages));

	auto &rendererContainer = GetRendererContainer();
	rendererContainer.Clear();

	//rendererContainer.Add<RendererShadows>(Pipeline::Stage(0, 0));

	rendererContainer.Add<RendererMeshes>(Pipeline::Stage(1, 0));

	rendererContainer.Add<RendererDeferred>(Pipeline::Stage(1, 1));
	rendererContainer.Add<RendererParticles>(Pipeline::Stage(1, 1));

	//rendererContainer.Add<FilterFxaa>(Pipeline::Stage(1, 2));
	//rendererContainer.Add<FilterTone>(Pipeline::Stage(1, 2));
	//rendererContainer.Add<FilterSsao>(Pipeline::Stage(1, 2));
	//auto sceneBlur = rendererContainer.Add<PipelineBlur>(Pipeline::Stage(1, 2), 1.8f, FilterBlur::Type::_5, false, 0.6f, 1.0f);
	//rendererContainer.Add<FilterDof>(Pipeline::Stage(1, 2), sceneBlur, 1.11f);
	//rendererContainer.Add<FilterEmboss>(Pipeline::Stage(1, 2));
	//rendererContainer.Add<FilterCrt>(Pipeline::Stage(1, 2));
	//rendererContainer.Add<FilterLensflare>(Pipeline::Stage(1, 2));
	//rendererContainer.Add<FilterTiltshift>(Pipeline::Stage(1, 2));
	//rendererContainer.Add<FilterPixel>(Pipeline::Stage(1, 2), 8.0f);
	//rendererContainer.Add<FilterVignette>(Pipeline::Stage(1, 2));
	//rendererContainer.Add<FilterGrain>(Pipeline::Stage(1, 2));
	rendererContainer.Add<FilterDefault>(Pipeline::Stage(1, 2), true);
	//rendererContainer.Add<RendererGizmos>(Pipeline::Stage(1, 2));
	rendererContainer.Add<RendererGuis>(Pipeline::Stage(1, 2));
	rendererContainer.Add<RendererFonts>(Pipeline::Stage(1, 2));
}

void MainRenderer::Update()
{
	//auto renderpassCreate0 = Renderer::Get()->GetRenderStage(0);
	//renderpassCreate0->GetViewport().SetSize(Vector2ui(Shadows::Get()->GetShadowSize()));

	/*auto renderpassCreate1 = Renderer::Get()->GetRenderStage(1);

	if (Keyboard::Get()->GetKey(Key::L) != InputAction::Release)
	{
		renderpassCreate1->GetViewport().SetScale(Vector2f::One);
		renderpassCreate1->GetViewport().SetOffset(Vector2ui::Zero);
	}
	else
	{
		renderpassCreate1->GetViewport().SetScale(Vector2f(0.6f, 0.6f));
		renderpassCreate1->GetViewport().SetOffset(Vector2ui(100, 100));
	}*/

	//Renderer::Get()->GetRenderer<FilterVignette>(true)->SetEnabled(Keyboard::Get()->GetKey(KEY_I));
}
}
