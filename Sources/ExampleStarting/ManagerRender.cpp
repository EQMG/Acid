#include "ManagerRender.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Worlds/Worlds.hpp>

namespace Demo
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
	{
		4096, 4096, // width / height
		{
			Attachment(0, TypeImage, VK_FORMAT_R16_UNORM) // shadows
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

	ManagerRender::ManagerRender() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE}),
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		m_rendererShadows(new RendererShadows({0, 0})),
		m_rendererSkyboxes(new RendererSkyboxes({1, 0})),
		m_rendererTerrains(new RendererTerrains({1, 0})),
		m_rendererWaters(new RendererWaters({1, 0})),
		m_rendererEntities(new RendererEntities({1, 0})),
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

	ManagerRender::~ManagerRender()
	{
		delete m_rendererShadows;

		delete m_rendererSkyboxes;
		delete m_rendererTerrains;
		delete m_rendererWaters;
		delete m_rendererEntities;
	//	delete m_rendererParticles;

		delete m_rendererDeferred;
		delete m_filterFxaa;
		delete m_filterLensflare;
		delete m_filterTiltshift;
		delete m_filterGrain;
		delete m_rendererGuis;
		delete m_rendererFonts;
	}

	void ManagerRender::Render()
	{
		RenderPass0();
		RenderPass1();
	}

	void ManagerRender::RenderPass0()
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

	void ManagerRender::RenderPass1()
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
		m_rendererSkyboxes->Render(commandBuffer, m_infinity, *camera);
		m_rendererTerrains->Render(commandBuffer, m_infinity, *camera);
		m_rendererWaters->Render(commandBuffer, m_infinity, *camera);
		m_rendererEntities->Render(commandBuffer, m_infinity, *camera);
	//	m_rendererParticles->Render(commandBuffer, m_infinity, *camera);
		Renderer::Get()->NextSubpass(commandBuffer);

		// Subpass 1.
		m_rendererDeferred->Render(commandBuffer, m_infinity, *camera);
		Renderer::Get()->NextSubpass(commandBuffer);

		// Subpass 2.
		m_filterFxaa->RenderFilter(commandBuffer);
		m_filterLensflare->SetSunPosition(*Worlds::Get()->GetSunPosition());
		m_filterLensflare->SetSunHeight(Worlds::Get()->GetSunHeight());
		m_filterLensflare->RenderFilter(commandBuffer);
		m_filterTiltshift->RenderFilter(commandBuffer);
		//m_filterGrain->RenderFilter(commandBuffer);
		m_rendererGuis->Render(commandBuffer, m_infinity, *camera);
		m_rendererFonts->Render(commandBuffer, m_infinity, *camera);

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(commandBuffer, 1);
	}
}
