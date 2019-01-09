#include "MainRenderer.hpp"

#include <Devices/Keyboard.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>
#include <Meshes/RendererMeshes.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/RendererParticles.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Post/Filters/FilterSsao.hpp>
#include <Post/Filters/FilterTiltshift.hpp>
#include <Post/Filters/FilterVignette.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/RendererShadows.hpp>

namespace test
{
	MainRenderer::MainRenderer()
	{
		std::vector<Attachment> renderpassImages0 = {
			Attachment(0, "shadows", ATTACHMENT_TYPE_IMAGE, false, VK_FORMAT_R8_UNORM)
		};
		std::vector<SubpassType> renderpassSubpasses0 = {
			SubpassType(0, {0})
		};
		m_renderpassCreates.emplace_back(RenderpassCreate(renderpassImages0, renderpassSubpasses0, 4096, 4096));

		std::vector<Attachment> renderpassImages1 = {
			Attachment(0, "depth", ATTACHMENT_TYPE_DEPTH, false),
			Attachment(1, "swapchain", ATTACHMENT_TYPE_SWAPCHAIN),
			Attachment(2, "diffuse", ATTACHMENT_TYPE_IMAGE, false, VK_FORMAT_R8G8B8A8_UNORM),
			Attachment(3, "normals", ATTACHMENT_TYPE_IMAGE, false, VK_FORMAT_R16G16B16A16_SFLOAT),
			Attachment(4, "materials", ATTACHMENT_TYPE_IMAGE, false, VK_FORMAT_R8G8B8A8_UNORM),
			Attachment(5, "resolved", ATTACHMENT_TYPE_IMAGE, false, VK_FORMAT_R8G8B8A8_UNORM)
		};
		std::vector<SubpassType> renderpassSubpasses1 = {
			SubpassType(0, {0, 2, 3, 4}),
			SubpassType(1, {0, 5}),
			SubpassType(2, {0, 1})
		};
		m_renderpassCreates.emplace_back(RenderpassCreate(renderpassImages1, renderpassSubpasses1));
	}

	void MainRenderer::Start()
	{
		auto &rendererContainer = GetRendererContainer();
	//	rendererContainer.Add<RendererShadows>(GraphicsStage(0, 0));

		rendererContainer.Add<RendererMeshes>(GraphicsStage(1, 0));

		rendererContainer.Add<RendererDeferred>(GraphicsStage(1, 1), DEFERRED_SIMPLE);
		rendererContainer.Add<RendererParticles>(GraphicsStage(1, 1));

	//	rendererContainer.Add<FilterFxaa>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterLensflare>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterTiltshift>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterGrain>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterVignette>(GraphicsStage(1, 2));
	//	rendererContainer.Add<FilterSsao>(GraphicsStage(1, 2));
		rendererContainer.Add<FilterDefault>(GraphicsStage(1, 2), true);
		rendererContainer.Add<RendererGuis>(GraphicsStage(1, 2));
		rendererContainer.Add<RendererFonts>(GraphicsStage(1, 2));
	}

	void MainRenderer::Update()
	{
		auto &renderpassCreate0 = Renderer::Get()->GetRenderStage(0)->GetRenderpassCreate();
		renderpassCreate0.SetWidth(Shadows::Get()->GetShadowSize());
		renderpassCreate0.SetHeight(Shadows::Get()->GetShadowSize());

	//	Renderer::Get()->GetRenderer<FilterVignette>(true)->SetEnabled(Keyboard::Get()->GetKey(KEY_I));
	}
}
