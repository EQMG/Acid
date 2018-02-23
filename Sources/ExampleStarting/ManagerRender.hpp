#pragma once

#include <Shadows/RendererShadows.hpp>
#include <Skyboxes/RendererSkyboxes.hpp>
#include <Renderer/IManagerRender.hpp>
#include <Terrains/RendererTerrains.hpp>
#include <Waters/RendererWaters.hpp>
#include <Entities/RendererEntities.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterTiltshift.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>

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
		FilterGrain *m_filterGrain;
		RendererGuis *m_rendererGuis;
		RendererFonts *m_rendererFonts;
	public:
		ManagerRender();

		~ManagerRender();

		void Render() override;
	};
}
