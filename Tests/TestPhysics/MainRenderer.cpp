#include "MainRenderer.hpp"

#include <Fonts/SubrenderFonts.hpp>
#include <Gizmos/SubrenderGizmos.hpp>
#include <Guis/SubrenderGuis.hpp>
#include <Meshes/SubrenderMeshes.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/SubrenderParticles.hpp>
#include <Post/Deferred/SubrenderDeferred.hpp>
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
#include <Graphics/Graphics.hpp>
#include <Shadows/SubrenderShadows.hpp>
#include "Devices/Keyboard.hpp"

namespace test {
MainRenderer::MainRenderer() {
	std::vector<Attachment> renderpassAttachments0 = {
		{0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM}
	};
	std::vector<SubpassType> renderpassSubpasses0 = {
		{0, {0}}
	};
	AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0, Viewport({4096, 4096})));

	std::vector<Attachment> renderpassAttachments1 = {
		{0, "depth", Attachment::Type::Depth, false},
		{1, "swapchain", Attachment::Type::Swapchain},
		{2, "position", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT},
		{3, "diffuse", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM},
		{4, "normal", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT},
		{5, "material", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM},
		{6, "resolved", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM}
	};
	std::vector<SubpassType> renderpassSubpasses1 = {
		{0, {0, 2, 3, 4, 5}},
		{1, {0, 6}},
		{2, {0, 1}}
	};
	AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
}

void MainRenderer::Start() {
	//AddSubrender<RenderShadows>({0, 0});

	AddSubrender<SubrenderMeshes>({1, 0});

	AddSubrender<SubrenderDeferred>({1, 1});
	AddSubrender<SubrenderParticles>({1, 1});

	AddSubrender<FilterFxaa>({1, 2});
	//AddSubrender<FilterTone>({1, 2});
	//AddSubrender<FilterSsao>({1, 2});
	//auto sceneBlur = AddSubrender<PipelineBlur>({1, 2}, 1.8f, FilterBlur::Type::_5, false, 0.6f, 1.0f);
	//AddSubrender<FilterDof>({1, 2}, sceneBlur, 1.11f);
	//AddSubrender<FilterEmboss>({1, 2});
	//AddSubrender<FilterCrt>({1, 2});
	//AddSubrender<FilterLensflare>({1, 2});
	//AddSubrender<FilterTiltshift>({1, 2});
	//AddSubrender<FilterPixel>({1, 2}, 8.0f);
	//AddSubrender<FilterVignette>({1, 2});
	//AddSubrender<FilterGrain>({1, 2});
	AddSubrender<FilterDefault>({1, 2}, true);
	//AddSubrender<RendererGizmos>({1, 2});
	AddSubrender<SubrenderGuis>({1, 2});
	AddSubrender<SubrenderFonts>({1, 2});
}

void MainRenderer::Update() {
	//auto renderpassCreate0 = GetRenderStage(0);
	//renderpassCreate0->GetViewport().SetSize(Shadows::Get()->GetShadowSize());

	/*auto renderpassCreate1 = GetRenderStage(1);

	if (Keyboard::Get()->GetKey(Key::L) != InputAction::Release) {
		renderpassCreate1->GetViewport().SetScale(Vector2f::One);
		renderpassCreate1->GetViewport().SetOffset(Vector2ui::Zero);
	} else {
		renderpassCreate1->GetViewport().SetScale({0.6f, 0.6f});
		renderpassCreate1->GetViewport().SetOffset({100, 100});
	}*/

	//GetSubrender<FilterVignette>()->SetEnabled(Keyboard::Get()->GetKey(Key::I) != InputAction::Release);
}
}
