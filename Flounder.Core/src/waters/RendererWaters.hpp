#pragma once

#include "../deferred/rendererdeferred.hpp"
#include "../fbos/fbo.hpp"
#include "../renderer/irenderer.hpp"

#include "waters.hpp"

namespace Flounder
{
	class rendererwaters :
		public IRenderer
	{
	private:
		Fbo *m_fboReflection;
		RendererDeferred *m_rendererDeferred;

		shader *m_shader;
	public:
		rendererwaters();

		~rendererwaters();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void prepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void renderWater(water *object);

		void endRendering();
	public:
		Fbo *getFboReflection() const { return m_fboReflection; }

		RendererDeferred *getRendererDeferred() const { return m_rendererDeferred; }
	};
}
