#include "MainRenderer.hpp"

#include <Fonts/FontsSubrender.hpp>
#include <Gizmos/GizmosSubrender.hpp>
#include <Guis/GuisSubrender.hpp>
#include <Devices/Keyboard.hpp>
#include <Meshes/MeshesSubrender.hpp>
#include <Models/Shapes/SphereModel.hpp>
#include <Particles/ParticlesSubrender.hpp>
#include <Post/Deferred/DeferredSubrender.hpp>
#include <Post/Filters/BlitFilter.hpp>
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
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/ShadowsSubrender.hpp>

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
#if defined(ACID_RELOAD)
		{1, "swapchain", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM},
#else
		{1, "swapchain", Attachment::Type::Swapchain},
#endif
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

#if defined(ACID_RELOAD)
	std::vector<Attachment> renderpassAttachments2 = {
		{0, "swapchainReal", Attachment::Type::Swapchain}
	};
	std::vector<SubpassType> renderpassSubpasses2 = {
		{0, {0}}
	};
	AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments2, renderpassSubpasses2));
#endif
}

void MainRenderer::Start() {
	//AddSubrender<RenderShadows>(Pipeline::Stage(0, 0));

	AddSubrender<MeshesSubrender>(Pipeline::Stage(1, 0));

	AddSubrender<DeferredSubrender>(Pipeline::Stage(1, 1));
	AddSubrender<ParticlesSubrender>(Pipeline::Stage(1, 1));

	//AddSubrender<FxaaFilter>(Pipeline::Stage(1, 2));
	//AddSubrender<ToneFilter>(Pipeline::Stage(1, 2));
	//AddSubrender<SsaoFilter>(Pipeline::Stage(1, 2));
	//AddSubrender()->AddRenderer<BlurPipeline>(Pipeline::Stage(1, 2), 1.8f, BlurPipeline::Type::_5, false, 0.6f, 1.0f);
	//AddSubrender<DofFilter>(Pipeline::Stage(1, 2), sceneBlur, 1.11f);
	//AddSubrender<EmbossFilter>(Pipeline::Stage(1, 2));
	//AddSubrender<CrtFilter>(Pipeline::Stage(1, 2));
	//AddSubrender<LensflareFilter>(Pipeline::Stage(1, 2));
	//AddSubrender<TiltshiftFilter>(Pipeline::Stage(1, 2));
	//AddSubrender<PixelFilter>(Pipeline::Stage(1, 2), 8.0f);
	//AddSubrender<VignetteFilter>(Pipeline::Stage(1, 2));
	//AddSubrender<GrainFilter>(Pipeline::Stage(1, 2));
	AddSubrender<DefaultFilter>(Pipeline::Stage(1, 2), true);
	//AddSubrender<RenderGizmos>(Pipeline::Stage(1, 2));
	AddSubrender<GuisSubrender>(Pipeline::Stage(1, 2));
	AddSubrender<FontsSubrender>(Pipeline::Stage(1, 2));

#if defined(ACID_RELOAD)
	AddSubrender<BlitFilter>({2, 0});
#endif
}

void MainRenderer::Update() {
	//auto renderpassCreate1 = Renderer::Get()->GetRenderStage(1);
	//renderpassCreate1->GetViewport().SetScale(0.75f);

	//Renderer::Get()->GetRenderer<VignetteFilter>(true)->SetEnabled(Keyboard::Get()->GetKey(KEY_I));
	
#if defined(ACID_RELOAD)
	auto renderpassCreate1 = GetRenderStage(1);
	//renderpassCreate1->GetViewport().SetOffset({0.1f, 0.0f});

	if (Keyboard::Get()->GetKey(Key::Q) == InputAction::Release) {
		renderpassCreate1->GetViewport().SetScale({1.0f, 1.0f});
	} else {
		renderpassCreate1->GetViewport().SetScale({0.5f, 1.0f});
	}

	//Renderer::Get()->GetRenderer<VignetteFilter>(true)->SetEnabled(Keyboard::Get()->GetKey(Key::I));
#endif
}
}
