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
#include <Post/Pipelines/PipelineGaussian.hpp>
#include <Renderer/IManagerRender.hpp>
#include <Shadows/RendererShadows.hpp>

using namespace fl;

namespace test
{
	class MainRenderer :
		public IManagerRender
	{
	public:
		MainRenderer();

		~MainRenderer();

		void Update() override;
	};
}
