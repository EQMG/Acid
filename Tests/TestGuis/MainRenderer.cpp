#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>

namespace Demo
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
		{
			0, 0, // width / height
			{
				Attachment(1, TypeSwapchain) // swapchain
			}, // images
			{
				SubpassType(0, {0}),
			} // subpasses
		};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE}),
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		m_rendererGuis(new RendererGuis({1, 2})),
		m_rendererFonts(new RendererFonts({1, 2}))
	{
	}

	MainRenderer::~MainRenderer()
	{
		delete m_rendererGuis;
		delete m_rendererFonts;
	}

	void MainRenderer::Render()
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
		m_rendererGuis->Render(commandBuffer, m_infinity, *camera);
		m_rendererFonts->Render(commandBuffer, m_infinity, *camera);

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(commandBuffer, 1);
	}
}
