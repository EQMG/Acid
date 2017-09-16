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

void ManagerRender::CreateCommands(size_t i, std::vector<VkCommandBuffer> commandBuffers)
{
	m_rendererTest->CreateCommands(i, commandBuffers);
}

void ManagerRender::Render()
{
	Renderer::Get()->BeginReindering();
	{
		ICamera *camera = Camera::Get()->GetCamera();

		m_rendererTest->Render(m_infinity, *camera);
		Renderer::Get()->NextSubpass();
	}
	Renderer::Get()->EndRendering();
}
