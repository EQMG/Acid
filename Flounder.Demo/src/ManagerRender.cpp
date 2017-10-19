#include "ManagerRender.hpp"

namespace Demo
{
	ManagerRender::ManagerRender() :
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		m_rendererSkyboxes(new RendererSkyboxes()),
		m_rendererWaters(new RendererWaters()),
		m_rendererTerrains(new RendererTerrains()),
		m_rendererTest(new RendererTest()),
		m_rendererGuis(new RendererGuis())
	{
	}

	ManagerRender::~ManagerRender()
	{
		delete m_rendererSkyboxes;
		delete m_rendererWaters;
		delete m_rendererTerrains;
		delete m_rendererTest;
		delete m_rendererGuis;
	}

	void ManagerRender::Render(const VkCommandBuffer *commandBuffer)
	{
		const auto camera = Camera::Get()->GetCamera();

		m_rendererSkyboxes->Render(commandBuffer, m_infinity, *camera);
		m_rendererTerrains->Render(commandBuffer, m_infinity, *camera);
		m_rendererWaters->Render(commandBuffer, m_infinity, *camera);
		m_rendererTest->Render(commandBuffer, m_infinity, *camera);

		m_rendererGuis->Render(commandBuffer, m_infinity, *camera);
	}
}
