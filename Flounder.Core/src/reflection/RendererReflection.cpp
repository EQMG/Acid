#include "RendererReflection.hpp"

#include "../camera/Camera.hpp"

namespace Flounder
{
	RendererReflection::RendererReflection(const int &subpass) :
		IRenderer(),
		m_clipPlane(Vector4(0.0f, 1.0f, 0.0f, 1.0f)),
		m_rendererSkyboxes(new RendererSkyboxes(subpass)),
		m_rendererTerrains(new RendererTerrains(subpass)),
		m_rendererEntities(new RendererEntities(subpass))
	{
	}

	RendererReflection::~RendererReflection()
	{
		delete m_rendererSkyboxes;
		delete m_rendererTerrains;
		delete m_rendererEntities;
	}

	void RendererReflection::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		Camera::Get()->GetCamera()->ReflectView(0.0f);

		m_rendererSkyboxes->Render(commandBuffer, m_clipPlane, camera);
		m_rendererTerrains->Render(commandBuffer, m_clipPlane, camera);
		m_rendererEntities->Render(commandBuffer, m_clipPlane, camera);

		Camera::Get()->GetCamera()->ReflectView(0.0f);
	}
}
