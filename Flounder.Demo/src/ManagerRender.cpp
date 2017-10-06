#include "managerrender.hpp"

ManagerRender::ManagerRender() :
	m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
	m_rendererSkyboxes(new RendererSkyboxes()),
	m_rendererTest(new RendererTest())
{
}

ManagerRender::~ManagerRender()
{
	delete m_rendererTest;
}

void ManagerRender::Render(const VkCommandBuffer *commandBuffer)
{
	ICamera *camera = Camera::Get()->GetCamera();

	m_rendererSkyboxes->Render(commandBuffer, m_infinity, *camera);
	m_rendererTest->Render(commandBuffer, m_infinity, *camera);
}
