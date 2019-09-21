#include "EditorRenderer.hpp"

#include <Fonts/SubrenderFonts.hpp>
#include <Gizmos/SubrenderGizmos.hpp>
#include <Guis/SubrenderGuis.hpp>
#include <Devices/Keyboard.hpp>
#include <Meshes/SubrenderMeshes.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/SubrenderParticles.hpp>
#include <Post/Deferred/SubrenderDeferred.hpp>
#include <Post/Filters/FilterBlit.hpp>
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
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/SubrenderShadows.hpp>

namespace test {
EditorRenderer::EditorRenderer() {
	std::vector<Attachment> renderpassAttachments0 = {
		{0, "depth", Attachment::Type::Depth},
		{1, "swapchain", Attachment::Type::Swapchain}
	};
	std::vector<SubpassType> renderpassSubpasses0 = {
		{0, {0, 1}}
	};
	AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0));
}

void EditorRenderer::Start() {
	AddSubrender<SubrenderGuis>({0, 0});
	AddSubrender<SubrenderFonts>({0, 0});
}

void EditorRenderer::Update() {
}
}
