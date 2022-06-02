#include "EditorRenderer.hpp"

#include <Fonts/FontsSubrender.hpp>
#include <Gizmos/GizmosSubrender.hpp>
#include <Guis/GuisSubrender.hpp>
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
	AddSubrender<GuisSubrender>({0, 0});
	AddSubrender<FontsSubrender>({0, 0});
}

void EditorRenderer::Update() {
}
}
