#pragma once

#include "../deferred/rendererdeferred.hpp"
#include "../fbos/fbo.hpp"
#include "../renderer/irenderer.hpp"

#include "terrains.hpp"

namespace Flounder
{
	class rendererterrains :
		public IRenderer
	{
	private:
		shader *m_shader;
	public:
		rendererterrains();

		~rendererterrains();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void prepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void renderTerrain(terrain *object);

		void endRendering();
	};
}
