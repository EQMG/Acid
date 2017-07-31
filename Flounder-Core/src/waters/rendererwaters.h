#pragma once

#include "../deferred/rendererdeferred.h"
#include "../fbos/fbo.h"
#include "../renderer/irenderer.h"

#include "waters.h"

namespace flounder
{
	class rendererwaters :
		public irenderer
	{
	private:
		fbo *m_fboReflection;
		rendererdeferred *m_rendererDeferred;

		shader *m_shader;
	public:
		rendererwaters();

		~rendererwaters();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderWater(water *object);

		void endRendering();
	public:
		fbo *getFboReflection() const { return m_fboReflection; }

		rendererdeferred *getRendererDeferred() const { return m_rendererDeferred; }
	};
}
