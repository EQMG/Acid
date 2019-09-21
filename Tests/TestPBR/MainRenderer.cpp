#include "MainRenderer.hpp"

#include <Fonts/SubrenderFonts.hpp>
#include <Guis/SubrenderGuis.hpp>
#include <Meshes/SubrenderMeshes.hpp>
#include <Particles/SubrenderParticles.hpp>
#include <Post/Deferred/SubrenderDeferred.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Graphics/Graphics.hpp>
#include <Shadows/SubrenderShadows.hpp>

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

	AddSubrender<SubrenderMeshes>({1, 0});

	AddSubrender<SubrenderDeferred>({1, 1});
	AddSubrender<SubrenderParticles>({1, 1});

	AddSubrender<FilterDefault>({1, 2}, true);
	AddSubrender<SubrenderGuis>({1, 2});
	AddSubrender<SubrenderFonts>({1, 2});
}

void MainRenderer::Update() {
}
}
