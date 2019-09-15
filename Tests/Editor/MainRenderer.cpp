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
#include "Filters/FilterBlit.hpp"

namespace test
{
MainRenderer::MainRenderer()
{
	std::vector<std::unique_ptr<RenderStage>> renderStages;

	std::vector<Attachment> renderpassAttachments0 = { 
		{0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM}
	};
	std::vector<SubpassType> renderpassSubpasses0 = {
		{0, {0}}
	};
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0, Viewport(Vector2ui(4096, 4096))));

	std::vector<Attachment> renderpassAttachments1 = {
		{0, "depth", Attachment::Type::Depth, false},
		{1, "swapchain", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM}, // Attachment::Type::Swapchain
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

	std::vector<Attachment> renderpassAttachments2 = {
		{0, "swapchainReal", Attachment::Type::Swapchain}
	};
	std::vector<SubpassType> renderpassSubpasses2 = {
		{0, {0}}
	};
	renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments2, renderpassSubpasses2));
	Graphics::Get()->SetRenderStages(std::move(renderStages));

	Graphics::Get()->ClearSubrenders();
	//Graphics::Get()->AddSubrender<RenderShadows>({0, 0});

	Graphics::Get()->AddSubrender<SubrenderMeshes>({1, 0});

	Graphics::Get()->AddSubrender<SubrenderDeferred>({1, 1});
	Graphics::Get()->AddSubrender<SubrenderParticles>({1, 1});

	//Graphics::Get()->AddSubrender<FilterFxaa>({1, 2});
	//Graphics::Get()->AddSubrender<FilterTone>({1, 2});
	//Graphics::Get()->AddSubrender<FilterSsao>({1, 2});
	//Graphics::Get()->AddRenderer<PipelineBlur>({1, 2}, 1.8f, PipelineBlur::Type::_5, false, 0.6f, 1.0f);
	//Graphics::Get()->AddSubrender<FilterDof>({1, 2}, sceneBlur, 1.11f);
	//Graphics::Get()->AddSubrender<FilterEmboss>({1, 2});
	//Graphics::Get()->AddSubrender<FilterCrt>({1, 2});
	//Graphics::Get()->AddSubrender<FilterLensflare>({1, 2});
	//Graphics::Get()->AddSubrender<FilterTiltshift>({1, 2});
	//Graphics::Get()->AddSubrender<FilterPixel>({1, 2}, 8.0f);
	//Graphics::Get()->AddSubrender<FilterVignette>({1, 2});
	//Graphics::Get()->AddSubrender<FilterGrain>({1, 2});
	Graphics::Get()->AddSubrender<FilterDefault>({1, 2}, true);
	//Graphics::Get()->AddSubrender<RenderGizmos>({1, 2});

	Graphics::Get()->AddSubrender<FilterBlit>({2, 0});
	Graphics::Get()->AddSubrender<SubrenderGuis>({2, 0});
	Graphics::Get()->AddSubrender<SubrenderFonts>({2, 0});
}

void MainRenderer::Update()
{
	auto renderpassCreate1 = Graphics::Get()->GetRenderStage(1);
	//renderpassCreate1->GetViewport().SetOffset({0.1f, 0.0f});

	if (Keyboard::Get()->GetKey(Key::Q) == InputAction::Release)
	{
		renderpassCreate1->GetViewport().SetScale({1.0f, 1.0f});
	}
	else
	{
		renderpassCreate1->GetViewport().SetScale({0.5f, 1.0f});
	}

	//Renderer::Get()->GetRenderer<FilterVignette>(true)->SetEnabled(Keyboard::Get()->GetKey(Key::I));
}
}
