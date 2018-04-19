#pragma once

#include <Meshes/RendererMeshes.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>
#include <Particles/RendererParticles.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Post/Filters/FilterTiltshift.hpp>
#include <Renderer/IManagerRender.hpp>
#include <Shadows/RendererShadows.hpp>
#include <Skyboxes/RendererSkyboxes.hpp>
#include <Terrains/RendererTerrains.hpp>
#include <Voxels/RendererVoxels.hpp>
#include <Waters/RendererWaters.hpp>

using namespace Flounder;

namespace Demo
{
	class MainRenderer :
		public IManagerRender
	{
	private:
		Vector4 m_infinity;

		RendererShadows *m_rendererShadows;

		RendererSkyboxes *m_rendererSkyboxes;
		RendererTerrains *m_rendererTerrains;
		RendererVoxels *m_rendererVoxels;
		RendererWaters *m_rendererWaters;
		RendererMeshes *m_rendererMeshes;
		//	RendererParticles *m_rendererParticles;

		RendererDeferred *m_rendererDeferred;
		FilterFxaa *m_filterFxaa;
		FilterLensflare *m_filterLensflare;
		FilterTiltshift *m_filterTiltshift;
		FilterGrain *m_filterGrain;
		RendererGuis *m_rendererGuis;
		RendererFonts *m_rendererFonts;
	public:
		MainRenderer();

		~MainRenderer();

		void Render() override;

	private:
		void RenderPass0();

		void RenderPass1();
	};
}
