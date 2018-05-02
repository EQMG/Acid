#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>

namespace Demo
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
		{
			4096, 4096, // width / height
			{
				Attachment(0, TypeImage, VK_FORMAT_R8_UNORM) // shadows
			}, // images
			{
				SubpassType(0, {0})
			} // subpasses
		};
	RenderpassCreate *RENDERPASS_1_CREATE = new RenderpassCreate
		{
			0, 0, // width / height
			{
				Attachment(0, TypeDepth), // depth
				Attachment(1, TypeSwapchain), // swapchain
				Attachment(2, TypeImage, VK_FORMAT_R8G8B8A8_UNORM), // colours
				Attachment(3, TypeImage, VK_FORMAT_R16G16_UNORM), // normals
				Attachment(4, TypeImage, VK_FORMAT_R8G8B8A8_UNORM) // materials
			}, // images
			{
				SubpassType(0, {0, 2, 3, 4}),
				SubpassType(1, {1}),
				SubpassType(2, {1}),
			} // subpasses
		};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE}),
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		m_rendererShadows(new RendererShadows({0, 0})),
		m_rendererMeshes(new RendererMeshes({1, 0})),
		//	m_rendererParticles(new RendererParticles({1, 0})),
		m_rendererDeferred(new RendererDeferred({1, 1})),
		m_filterFxaa(new FilterFxaa({1, 2})),
		m_filterLensflare(new FilterLensflare({1, 2})),
		m_filterTiltshift(new FilterTiltshift({1, 2})),
		m_filterGrain(new FilterGrain({1, 2})),
		m_rendererGuis(new RendererGuis({1, 2})),
		m_rendererFonts(new RendererFonts({1, 2}))
	{
	}

	MainRenderer::~MainRenderer()
	{
		delete m_rendererShadows;

		delete m_rendererMeshes;
		//	delete m_rendererParticles;

		delete m_rendererDeferred;
		delete m_filterFxaa;
		delete m_filterLensflare;
		delete m_filterTiltshift;
		delete m_filterGrain;
		delete m_rendererGuis;
		delete m_rendererFonts;
	}

	void MainRenderer::Render()
	{
		RenderPass0();
		RenderPass1();
	}

	void MainRenderer::RenderPass0()
	{
		RENDERPASS_0_CREATE->m_width = Shadows::Get()->GetShadowSize();
		RENDERPASS_0_CREATE->m_height = Shadows::Get()->GetShadowSize();

		const auto commandBuffer = Renderer::Get()->GetCommandBuffer();
		const auto camera = Scenes::Get()->GetCamera();

		// Starts Rendering.
		VkResult startResult = Renderer::Get()->StartRenderpass(commandBuffer, 0);

		if (startResult != VK_SUCCESS)
		{
			return;
		}

		// Subpass 0.
		m_rendererShadows->Render(commandBuffer, m_infinity, *camera);

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(commandBuffer, 0);
	}

	void MainRenderer::RenderPass1()
	{
		const auto commandBuffer = Renderer::Get()->GetCommandBuffer();
		const auto camera = Scenes::Get()->GetCamera();

		// Starts Rendering.
		VkResult startResult = Renderer::Get()->StartRenderpass(commandBuffer, 1);

		if (startResult != VK_SUCCESS)
		{
			return;
		}

		// Subpass 0.
		m_rendererMeshes->Render(commandBuffer, m_infinity, *camera);
		//	m_rendererParticles->Render(commandBuffer, m_infinity, *camera);
		Renderer::Get()->NextSubpass(commandBuffer);

		// Subpass 1.
		m_rendererDeferred->Render(commandBuffer, m_infinity, *camera);
		Renderer::Get()->NextSubpass(commandBuffer);

		// Subpass 2.
#ifndef FL_BUILD_MACOS
		m_filterFxaa->Render(commandBuffer);
//		m_filterLensflare->SetSunPosition(*Worlds::Get()->GetSunPosition());
//		m_filterLensflare->SetSunHeight(Worlds::Get()->GetSunHeight());
//		m_filterLensflare->Render(commandBuffer);
//		m_filterTiltshift->Render(commandBuffer);
//		m_filterGrain->Render(commandBuffer);
#endif
		m_rendererGuis->Render(commandBuffer, m_infinity, *camera);
		m_rendererFonts->Render(commandBuffer, m_infinity, *camera);

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(commandBuffer, 1);
	}
}
