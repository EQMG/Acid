#include "MainRenderer.hpp"

#include <Fonts/RenderFonts.hpp>
#include <Gizmos/RenderGizmos.hpp>
#include <Guis/RenderGuis.hpp>
#include <Devices/Keyboard.hpp>
#include <Meshes/RenderMeshes.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/RenderParticles.hpp>
#include <Post/Deferred/RenderDeferred.hpp>
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
#include <Shadows/RenderShadows.hpp>

namespace test
{
MainRenderer::MainRenderer()
{
	std::vector<std::unique_ptr<RenderStage>> renderStages;

	std::vector<Attachment> renderpassAttachments0 = { Attachment(0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM) };
	std::vector<SubpassType> renderpassSubpasses0 = { SubpassType(0, { 0 }) };
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0, Viewport(Vector2ui(4096, 4096))));

	std::vector<Attachment> renderpassAttachments1 = { Attachment(0, "depth", Attachment::Type::Depth, false), Attachment(1, "swapchain", Attachment::Type::Swapchain),
		Attachment(2, "position", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
		Attachment(3, "diffuse", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM), Attachment(4, "normal", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
		Attachment(5, "material", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM), Attachment(6, "resolved", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM) };
	std::vector<SubpassType> renderpassSubpasses1 = { SubpassType(0, { 0, 2, 3, 4, 5 }), SubpassType(1, { 0, 6 }), SubpassType(2, { 0, 1 }) };
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
	Renderer::Get()->SetRenderStages(std::move(renderStages));
	
	auto &renderHolder = GetRenderHolder();
	//renderHolder.Add<RenderShadows>(Pipeline::Stage(0, 0));

	renderHolder.Add<RenderMeshes>(Pipeline::Stage(1, 0));

	renderHolder.Add<RenderDeferred>(Pipeline::Stage(1, 1));
	renderHolder.Add<RenderParticles>(Pipeline::Stage(1, 1));

	//renderHolder.Add<FilterFxaa>(Pipeline::Stage(1, 2));
	//renderHolder.Add<FilterTone>(Pipeline::Stage(1, 2));
	//renderHolder.Add<FilterSsao>(Pipeline::Stage(1, 2));
	//renderHolder.Add()->AddRenderer<PipelineBlur>(Pipeline::Stage(1, 2), 1.8f, PipelineBlur::Type::_5, false, 0.6f, 1.0f);
	//renderHolder.Add<FilterDof>(Pipeline::Stage(1, 2), sceneBlur, 1.11f);
	//renderHolder.Add<FilterEmboss>(Pipeline::Stage(1, 2));
	//renderHolder.Add<FilterCrt>(Pipeline::Stage(1, 2));
	//renderHolder.Add<FilterLensflare>(Pipeline::Stage(1, 2));
	//renderHolder.Add<FilterTiltshift>(Pipeline::Stage(1, 2));
	//renderHolder.Add<FilterPixel>(Pipeline::Stage(1, 2), 8.0f);
	//renderHolder.Add<FilterVignette>(Pipeline::Stage(1, 2));
	//renderHolder.Add<FilterGrain>(Pipeline::Stage(1, 2));
	renderHolder.Add<FilterDefault>(Pipeline::Stage(1, 2), true);
	//renderHolder.Add<RenderGizmos>(Pipeline::Stage(1, 2));
	renderHolder.Add<RenderGuis>(Pipeline::Stage(1, 2));
	renderHolder.Add<RenderFonts>(Pipeline::Stage(1, 2));
}

void MainRenderer::Update()
{
	//auto renderpassCreate1 = Renderer::Get()->GetRenderStage(1);
	//renderpassCreate1->GetViewport().SetScale(0.75f);

	//Renderer::Get()->GetRenderer<FilterVignette>(true)->SetEnabled(Keyboard::Get()->GetKey(KEY_I));
}
}
