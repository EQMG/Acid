#pragma once

#include "../deferred/rendererdeferred.hpp"
#include "../fbos/fbo.hpp"
#include "../renderer/irenderer.hpp"

#include "waters.hpp"

namespace flounder
{
	class rendererwaters :
		public irenderer
	{
	private:
		Fbo *m_fboReflection;
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
		Fbo *getFboReflection() const { return m_fboReflection; }

		rendererdeferred *getRendererDeferred() const { return m_rendererDeferred; }
	};
}
