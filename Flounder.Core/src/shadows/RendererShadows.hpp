#pragma once

#include "../deferred/rendererdeferred.hpp"
#include "../fbos/fbo.hpp"
#include "../renderer/irenderer.hpp"
#include "../terrains/terrains.hpp"

#include "shadows.hpp"

namespace Flounder
{
	class renderershadows :
		public irenderer
	{
	private:
		Fbo *m_fbo;
		shader *m_shader;
	public:
		renderershadows();

		~renderershadows();

		void render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void prepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void renderModel(model *object, Matrix4 *modelMatrix);

		void endRendering();
	public:
		Fbo *getFbo() const { return m_fbo; }
	};
}
