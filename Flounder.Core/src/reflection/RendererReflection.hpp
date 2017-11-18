#pragma once

#include "../renderer/IRenderer.hpp"
#include "../entities/RendererEntities.hpp"
#include "../terrains/RendererTerrains.hpp"
#include "../skyboxes/RendererSkyboxes.hpp"
#include "../waters/RendererWaters.hpp"

namespace Flounder
{
	class RendererReflection :
		public IRenderer
	{
	private:
		Vector4 m_clipPlane;

		RendererSkyboxes *m_rendererSkyboxes;
		RendererTerrains *m_rendererTerrains;
		RendererEntities *m_rendererEntities;
	public:
		RendererReflection(const int &subpass);

		~RendererReflection();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
