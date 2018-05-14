#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>

namespace Demo
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
		{
			0, 0, // width / height
			{
				Attachment(1, AttachmentType::ATTACHMENT_SWAPCHAIN) // swapchain
			}, // images
			{
				SubpassType(0, {0}),
			} // subpasses
		};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE}),
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		m_rendererGuis(new RendererGuis({0, 0})),
		m_rendererFonts(new RendererFonts({0, 0}))
	{
	}

	MainRenderer::~MainRenderer()
	{
		delete m_rendererGuis;
		delete m_rendererFonts;
	}

	void MainRenderer::Render()
	{
		const auto commandBuffer = Renderer::Get()->GetVkCommandBuffer();
		const auto camera = Scenes::Get()->GetCamera();

		// Starts Rendering.
		auto startResult = Renderer::Get()->StartRenderpass(commandBuffer, 0);

		if (!startResult)
		{
			return;
		}

		// Subpass 0.
		m_rendererGuis->Render(commandBuffer, m_infinity, *camera);
		m_rendererFonts->Render(commandBuffer, m_infinity, *camera);

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(commandBuffer, 0);
	}
}
