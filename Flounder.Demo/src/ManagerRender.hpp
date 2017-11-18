#pragma once

#include <Flounder.hpp>

using namespace Flounder;

namespace Demo
{
	class ManagerRender :
		public IManagerRender
	{
	private:
		Vector4 m_infinity;

		// RendererShadows *m_rendererShadows;
		RendererReflection *m_rendererReflection;

		RendererSkyboxes *m_rendererSkyboxes;
		RendererTerrains *m_rendererTerrains;
		RendererWaters *m_rendererWaters;
		RendererEntities *m_rendererEntities;

		RendererDeferred *m_rendererDeferred;
		FilterGrey *m_filterGrey;
		RendererGuis *m_rendererGuis;
		RendererFonts *m_rendererFonts;
	public:
		ManagerRender();

		~ManagerRender();

		void Render(const VkCommandBuffer *commandBuffer) override;
	};
}
