#pragma once

#include "../deferred/RendererDeferred.hpp"
#include "../fbos/Fbo.hpp"
#include "../renderer/IRenderer.hpp"
#include "../terrains/Terrains.hpp"

#include "Shadows.hpp"

namespace Flounder
{
	class RendererShadows :
		public IRenderer
	{
	private:
		Fbo *m_fbo;
		Shader *m_shader;
	public:
		RendererShadows();

		~RendererShadows();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void RenderModel(Model *object, Matrix4 *modelMatrix);

		void EndRendering();
	public:
		Fbo *GetFbo() const { return m_fbo; }
	};
}
