#include "MainRenderer.hpp"

#include <Fonts/FontsSubrender.hpp>
#include <Guis/GuisSubrender.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>

namespace test {
MainRenderer::MainRenderer() {
	std::vector<Attachment> renderpassAttachments0 = {
		{0, "depth", Attachment::Type::Depth},
		{1, "swapchain", Attachment::Type::Swapchain, false, VK_FORMAT_R8G8B8A8_UNORM, Colour::White}
	};
	std::vector<SubpassType> renderpassSubpasses0 = {
		{0, {0, 1}}
	};
	AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0));
}

void MainRenderer::Start() {
	AddSubrender<GuisSubrender>({0, 0});
	AddSubrender<FontsSubrender>({0, 0});
}

void MainRenderer::Update() {
}
}
