#pragma once

#include "../deferred/RendererDeferred.hpp"
#include "../fbos/Fbo.hpp"
#include "../renderer/IRenderer.hpp"

#include "Waters.hpp"

namespace Flounder
{
	class RendererWaters :
		public IRenderer
	{
	private:
		Fbo *m_fboReflection;
		RendererDeferred *m_rendererDeferred;

		Shader *m_shader;
	public:
		RendererWaters();

		~RendererWaters();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void RenderWater(Water *object);

		void EndRendering();
	public:
		Fbo *GetFboReflection() const { return m_fboReflection; }

		RendererDeferred *GetRendererDeferred() const { return m_rendererDeferred; }
	};
}
