#include "MainRenderer.hpp"

#include <Devices/Keyboard.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>
#include <Meshes/RendererMeshes.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/RendererParticles.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/RendererShadows.hpp>

namespace test
{
	MainRenderer::MainRenderer()
	{
		std::vector<Attachment> renderpassImages0 = {
			Attachment(0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM)
		};
		std::vector<SubpassType> renderpassSubpasses0 = {
			SubpassType(0, {0})
		};
		m_renderpassCreates.emplace_back(RenderpassCreate(renderpassImages0, renderpassSubpasses0, 4096, 4096));

		std::vector<Attachment> renderpassImages1 = {
			Attachment(0, "depth", Attachment::Type::Depth, false),
			Attachment(1, "swapchain", Attachment::Type::Swapchain),
			Attachment(2, "diffuse", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM),
			Attachment(3, "normals", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT),
			Attachment(4, "materials", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM),
			Attachment(5, "resolved", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM)
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

		rendererContainer.Add<RendererDeferred>(GraphicsStage(1, 1), RendererDeferred::Type::Ibl);
		rendererContainer.Add<RendererParticles>(GraphicsStage(1, 1));

		rendererContainer.Add<FilterDefault>(GraphicsStage(1, 2), true);
		rendererContainer.Add<RendererGuis>(GraphicsStage(1, 2));
		rendererContainer.Add<RendererFonts>(GraphicsStage(1, 2));
	}

	void MainRenderer::Update()
	{
		auto &renderpassCreate0 = Renderer::Get()->GetRenderStage(0)->GetRenderpassCreate();
		renderpassCreate0.SetWidth(Shadows::Get()->GetShadowSize());
		renderpassCreate0.SetHeight(Shadows::Get()->GetShadowSize());
	}
}
