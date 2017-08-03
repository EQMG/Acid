#pragma once

#include "../deferred/rendererdeferred.h"
#include "../fbos/fbo.h"
#include "../renderer/irenderer.h"

#include "terrains.h"

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
