#pragma once

#include <Flounder.hpp>
#include "deferred/RendererDeferred.hpp"

using namespace Flounder;

namespace Demo
{
	class ManagerRender :
		public IManagerRender
	{
	private:
		Vector4 m_infinity;

		RendererSkyboxes *m_rendererSkyboxes;
		RendererWaters *m_rendererWaters;
		RendererTerrains *m_rendererTerrains;
		RendererTest *m_rendererTest;
		RendererGuis *m_rendererGuis;
		RendererFonts *m_rendererFonts;

		RendererDeferred *m_rendererDeferred;
		FilterGrey *m_filterGrey;
	public:
		ManagerRender();

		~ManagerRender();

		void Render(const VkCommandBuffer *commandBuffer) override;
	};
}
