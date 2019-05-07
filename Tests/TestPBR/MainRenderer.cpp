#include "MainRenderer.hpp"

#include <Fonts/SubrenderFonts.hpp>
#include <Guis/SubrenderGuis.hpp>
#include <Meshes/SubrenderMeshes.hpp>
#include <Particles/SubrenderParticles.hpp>
#include <Post/Deferred/SubrenderDeferred.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Renderer/Renderer.hpp>
#include <Shadows/SubrenderShadows.hpp>

namespace test
{
MainRenderer::MainRenderer()
{
	std::vector<std::unique_ptr<RenderStage>> renderStages;

	std::vector<Attachment> renderpassAttachments0 = { 
		Attachment(0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM) 
	};
	std::vector<SubpassType> renderpassSubpasses0 = { 
		SubpassType(0, { 0 }) 
	};
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0, Viewport(Vector2ui(4096, 4096))));

	std::vector<Attachment> renderpassAttachments1 = { 
		Attachment(0, "depth", Attachment::Type::Depth, false), 
		Attachment(1, "swapchain", Attachment::Type::Swapchain),
		Attachment(2, "position", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
		Attachment(3, "diffuse", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM), 
		Attachment(4, "normal", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
		Attachment(5, "material", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM), 
		Attachment(6, "resolved", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM) 
	};
	std::vector<SubpassType> renderpassSubpasses1 = { 
		SubpassType(0, { 0, 2, 3, 4, 5 }), 
		SubpassType(1, { 0, 6 }), 
		SubpassType(2, { 0, 1 }) 
	};
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
	Renderer::Get()->SetRenderStages(std::move(renderStages));

	auto &renderHolder = GetRenderHolder();
	//renderHolder.Add<RenderShadows>(Pipeline::Stage(0, 0));

	renderHolder.Add<SubrenderMeshes>(Pipeline::Stage(1, 0));

	renderHolder.Add<SubrenderDeferred>(Pipeline::Stage(1, 1));
	renderHolder.Add<SubrenderParticles>(Pipeline::Stage(1, 1));

	renderHolder.Add<FilterDefault>(Pipeline::Stage(1, 2), true);
	renderHolder.Add<SubrenderGuis>(Pipeline::Stage(1, 2));
	renderHolder.Add<SubrenderFonts>(Pipeline::Stage(1, 2));
}

void MainRenderer::Update()
{
}
}
