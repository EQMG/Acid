#include "ManagerRender.hpp"

namespace Demo
{
	ManagerRender::ManagerRender() :
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		// m_rendererShadows(new RendererShadows(0)),
		m_rendererReflection(new RendererReflection(0)),
		m_rendererSkyboxes(new RendererSkyboxes(1)),
		m_rendererTerrains(new RendererTerrains(1)),
		m_rendererWaters(new RendererWaters(1)),
		m_rendererEntities(new RendererEntities(1)),
		m_rendererDeferred(new RendererDeferred(2)),
		m_filterGrey(new FilterGrey(2)),
		m_rendererGuis(new RendererGuis(2)),
		m_rendererFonts(new RendererFonts(2))
	{
	}

	ManagerRender::~ManagerRender()
	{
		// delete m_rendererShadows;
		delete m_rendererReflection;

		delete m_rendererSkyboxes;
		delete m_rendererTerrains;
		delete m_rendererWaters;
		delete m_rendererEntities;

		delete m_rendererDeferred;
		delete m_filterGrey;
		delete m_rendererGuis;
		delete m_rendererFonts;
	}

	void ManagerRender::Render(const VkCommandBuffer *commandBuffer)
	{
		const auto camera = Camera::Get()->GetCamera();

		// Subpass 0
		// m_rendererShadows->Render(commandBuffer, m_infinity, *camera);
		m_rendererReflection->Render(commandBuffer, m_infinity, *camera);
		vkCmdNextSubpass(*commandBuffer, VK_SUBPASS_CONTENTS_INLINE);

		// Subpass 1
		m_rendererSkyboxes->Render(commandBuffer, m_infinity, *camera);
		m_rendererTerrains->Render(commandBuffer, m_infinity, *camera);
		m_rendererWaters->Render(commandBuffer, m_infinity, *camera);
		m_rendererEntities->Render(commandBuffer, m_infinity, *camera);
		vkCmdNextSubpass(*commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
		
		// Subpass 2
		m_rendererDeferred->Render(commandBuffer, m_infinity, *camera);
	//	m_filterGrey->RenderFilter(commandBuffer);
		m_rendererGuis->Render(commandBuffer, m_infinity, *camera);
		m_rendererFonts->Render(commandBuffer, m_infinity, *camera);
	}
}
