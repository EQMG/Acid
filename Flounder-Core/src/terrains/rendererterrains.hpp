#pragma once

#include "../deferred/rendererdeferred.hpp"
#include "../fbos/fbo.hpp"
#include "../renderer/irenderer.hpp"

#include "terrains.hpp"

namespace flounder
{
	class rendererterrains :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		rendererterrains();

		~rendererterrains();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderTerrain(terrain *object);

		void endRendering();
	};
}
