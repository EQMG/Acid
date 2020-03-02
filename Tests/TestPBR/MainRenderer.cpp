#include "MainRenderer.hpp"

#include <Fonts/FontsSubrender.hpp>
#include <Guis/GuisSubrender.hpp>
#include <Meshes/MeshesSubrender.hpp>
#include <Particles/ParticlesSubrender.hpp>
#include <Post/Deferred/DeferredSubrender.hpp>
#include <Post/Filters/DefaultFilter.hpp>
#include <Graphics/Graphics.hpp>
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

	std::vector<Attachment> renderpassAttachments1{
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

	AddSubrender<DefaultFilter>({1, 2}, true);
	AddSubrender<GuisSubrender>({1, 2});
	AddSubrender<FontsSubrender>({1, 2});
}

void MainRenderer::Update() {
}
}
