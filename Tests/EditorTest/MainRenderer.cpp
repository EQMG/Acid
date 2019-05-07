#include "MainRenderer.hpp"

#include <Fonts/SubrenderFonts.hpp>
#include <Gizmos/SubrenderGizmos.hpp>
#include <Guis/SubrenderGuis.hpp>
#include <Devices/Keyboard.hpp>
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
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
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
	Graphics::Get()->SetRenderStages(std::move(renderStages));

	Graphics::Get()->ClearSubrenders();
	//Graphics::Get()->AddSubrender<RenderShadows>(Pipeline::Stage(0, 0));

	Graphics::Get()->AddSubrender<SubrenderMeshes>(Pipeline::Stage(1, 0));

	Graphics::Get()->AddSubrender<SubrenderDeferred>(Pipeline::Stage(1, 1));
	Graphics::Get()->AddSubrender<SubrenderParticles>(Pipeline::Stage(1, 1));

	//Graphics::Get()->AddSubrender<FilterFxaa>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender<FilterTone>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender<FilterSsao>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender()->AddRenderer<PipelineBlur>(Pipeline::Stage(1, 2), 1.8f, PipelineBlur::Type::_5, false, 0.6f, 1.0f);
	//Graphics::Get()->AddSubrender<FilterDof>(Pipeline::Stage(1, 2), sceneBlur, 1.11f);
	//Graphics::Get()->AddSubrender<FilterEmboss>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender<FilterCrt>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender<FilterLensflare>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender<FilterTiltshift>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender<FilterPixel>(Pipeline::Stage(1, 2), 8.0f);
	//Graphics::Get()->AddSubrender<FilterVignette>(Pipeline::Stage(1, 2));
	//Graphics::Get()->AddSubrender<FilterGrain>(Pipeline::Stage(1, 2));
	Graphics::Get()->AddSubrender<FilterDefault>(Pipeline::Stage(1, 2), true);
	//Graphics::Get()->AddSubrender<RenderGizmos>(Pipeline::Stage(1, 2));
	Graphics::Get()->AddSubrender<SubrenderGuis>(Pipeline::Stage(1, 2));
	Graphics::Get()->AddSubrender<SubrenderFonts>(Pipeline::Stage(1, 2));
}

void MainRenderer::Update()
{
	//auto renderpassCreate1 = Renderer::Get()->GetRenderStage(1);
	//renderpassCreate1->GetViewport().SetScale(0.75f);

	//Renderer::Get()->GetRenderer<FilterVignette>(true)->SetEnabled(Keyboard::Get()->GetKey(KEY_I));
}
}
