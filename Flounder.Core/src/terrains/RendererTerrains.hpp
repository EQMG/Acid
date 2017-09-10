#pragma once

#include "../deferred/RendererDeferred.hpp"
#include "../fbos/Fbo.hpp"
#include "../renderer/IRenderer.hpp"

#include "Terrains.hpp"

namespace Flounder
{
	class RendererTerrains :
		public IRenderer
	{
	private:
		Shader *m_shader;
	public:
		RendererTerrains();

		~RendererTerrains();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void RenderTerrain(Terrain *object);

		void EndRendering();
	};
}
