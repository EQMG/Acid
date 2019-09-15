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
	std::vector<std::unique_ptr<RenderStage>> renderStages;

	std::vector<Attachment> renderpassAttachments0 = {
		{0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM}
	};
	std::vector<SubpassType> renderpassSubpasses0 = {
		{0, {0}}
	};
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0, Viewport({4096, 4096})));

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
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
	Graphics::Get()->SetRenderStages(std::move(renderStages));

	Graphics::Get()->ClearSubrenders();
	//Graphics::Get()->AddSubrender<RenderShadows>({0, 0});

	Graphics::Get()->AddSubrender<SubrenderMeshes>({1, 0});

	Graphics::Get()->AddSubrender<SubrenderDeferred>({1, 1});
	Graphics::Get()->AddSubrender<SubrenderParticles>({1, 1});

	Graphics::Get()->AddSubrender<FilterDefault>({1, 2}, true);
	Graphics::Get()->AddSubrender<SubrenderGuis>({1, 2});
	Graphics::Get()->AddSubrender<SubrenderFonts>({1, 2});
}

void MainRenderer::Update() {
}
}
