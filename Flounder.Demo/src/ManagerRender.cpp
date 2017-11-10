#include "ManagerRender.hpp"

namespace Demo
{
	ManagerRender::ManagerRender() :
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		m_rendererShadows(new RendererShadows()),
		m_rendererSkyboxes(new RendererSkyboxes()),
		m_rendererTerrains(new RendererTerrains()),
		m_rendererWaters(new RendererWaters()),
		m_rendererTest(new RendererTest()),
		m_rendererDeferred(new RendererDeferred()),
		m_filterGrey(new FilterGrey()),
		m_rendererGuis(new RendererGuis()),
		m_rendererFonts(new RendererFonts())
	{
	}

	ManagerRender::~ManagerRender()
	{
		delete m_rendererShadows;

		delete m_rendererSkyboxes;
		delete m_rendererTerrains;
		delete m_rendererWaters;
		delete m_rendererTest;

		delete m_rendererDeferred;
		delete m_filterGrey;
		delete m_rendererGuis;
		delete m_rendererFonts;
	}

	void ManagerRender::Render(const VkCommandBuffer *commandBuffer)
	{
		const auto camera = Camera::Get()->GetCamera();

		// Subpass 0
		m_rendererShadows->Render(commandBuffer, m_infinity, *camera);
		vkCmdNextSubpass(*commandBuffer, VK_SUBPASS_CONTENTS_INLINE);

		// Subpass 1
		m_rendererSkyboxes->Render(commandBuffer, m_infinity, *camera);
		m_rendererTerrains->Render(commandBuffer, m_infinity, *camera);
		m_rendererWaters->Render(commandBuffer, m_infinity, *camera);
		m_rendererTest->Render(commandBuffer, m_infinity, *camera);
		vkCmdNextSubpass(*commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
		
		// Subpass 2
		m_rendererDeferred->Render(commandBuffer, m_infinity, *camera);
	//	m_filterGrey->RenderFilter(commandBuffer);
		m_rendererGuis->Render(commandBuffer, m_infinity, *camera);
		m_rendererFonts->Render(commandBuffer, m_infinity, *camera);
	}
}
