#include "MainRenderer.hpp"

#include <Fonts/FontsSubrender.hpp>
#include <Gizmos/GizmosSubrender.hpp>
#include <Guis/GuisSubrender.hpp>
#include <Meshes/MeshesSubrender.hpp>
#include <Models/Shapes/SphereModel.hpp>
#include <Particles/ParticlesSubrender.hpp>
#include <Post/Deferred/DeferredSubrender.hpp>
#include <Post/Filters/CrtFilter.hpp>
#include <Post/Filters/DefaultFilter.hpp>
#include <Post/Filters/DofFilter.hpp>
#include <Post/Filters/EmbossFilter.hpp>
#include <Post/Filters/FxaaFilter.hpp>
#include <Post/Filters/GrainFilter.hpp>
#include <Post/Filters/LensflareFilter.hpp>
#include <Post/Filters/PixelFilter.hpp>
#include <Post/Filters/SsaoFilter.hpp>
#include <Post/Filters/TiltshiftFilter.hpp>
#include <Post/Filters/ToneFilter.hpp>
#include <Post/Filters/VignetteFilter.hpp>
#include <Post/Pipelines/BlurPipeline.hpp>
#include <Graphics/Graphics.hpp>
#include <Shadows/ShadowsSubrender.hpp>
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

	AddSubrender<MeshesSubrender>({1, 0});

	AddSubrender<DeferredSubrender>({1, 1});
	AddSubrender<ParticlesSubrender>({1, 1});

	//AddSubrender<FxaaFilter>({1, 2});
	//AddSubrender<ToneFilter>({1, 2});
	//AddSubrender<SsaoFilter>({1, 2});
	//auto sceneBlur = AddSubrender<BlurPipeline>({1, 2}, 1.8f, BlurFilter::Type::_5, false, 0.6f, 1.0f);
	//AddSubrender<DofFilter>({1, 2}, sceneBlur, 1.11f);
	//AddSubrender<EmbossFilter>({1, 2});
	//AddSubrender<CrtFilter>({1, 2});
	//AddSubrender<LensflareFilter>({1, 2});
	//AddSubrender<TiltshiftFilter>({1, 2});
	//AddSubrender<PixelFilter>({1, 2}, 8.0f);
	//AddSubrender<VignetteFilter>({1, 2});
	//AddSubrender<GrainFilter>({1, 2});
	AddSubrender<DefaultFilter>({1, 2}, true);
	//AddSubrender<RendererGizmos>({1, 2});
	AddSubrender<GuisSubrender>({1, 2});
	AddSubrender<FontsSubrender>({1, 2});
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

	//GetSubrender<VignetteFilter>()->SetEnabled(Keyboard::Get()->GetKey(Key::I) != InputAction::Release);
}
}
