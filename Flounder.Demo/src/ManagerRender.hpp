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

		RendererSkyboxes *m_rendererSkyboxes;
		RendererWaters *m_rendererWaters;
		RendererTerrains *m_rendererTerrains;
		RendererTest *m_rendererTest;
		RendererGuis *m_rendererGuis;
	public:
		ManagerRender();

		~ManagerRender();

		void Render(const VkCommandBuffer *commandBuffer) override;
	};
}
