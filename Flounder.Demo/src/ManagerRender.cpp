#include "managerrender.hpp"

ManagerRender::ManagerRender() :
	m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
	m_rendererTest()
{
}

ManagerRender::~ManagerRender()
{
	delete m_rendererTest;
}

void ManagerRender::Render(const VkCommandBuffer *commandBuffer)
{
	ICamera *camera = Camera::Get()->GetCamera();

	Renderer::Get()->BeginReindering();
	{
		if (m_rendererTest != nullptr)
		{
			m_rendererTest->Render(commandBuffer, m_infinity, *camera);
		}
	//	Renderer::Get()->NextSubpass();
	}
	Renderer::Get()->EndRendering();
}
