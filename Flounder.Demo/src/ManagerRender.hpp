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

		RendererShadows *m_rendererShadows;

		RendererSkyboxes *m_rendererSkyboxes;
		RendererTerrains *m_rendererTerrains;
		RendererWaters *m_rendererWaters;
		RendererEntities *m_rendererEntities;

		RendererDeferred *m_rendererDeferred;
		FilterFxaa *m_filterFxaa;
		FilterLensflare *m_filterLensflare;
		FilterTiltshift *m_filterTiltshift;
		RendererGuis *m_rendererGuis;
		RendererFonts *m_rendererFonts;
	public:
		ManagerRender();

		~ManagerRender();

		void Render(const VkCommandBuffer *commandBuffer) override;
	};
}
