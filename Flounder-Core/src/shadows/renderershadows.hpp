#pragma once

#include "../deferred/rendererdeferred.hpp"
#include "../fbos/fbo.hpp"
#include "../renderer/irenderer.hpp"
#include "../terrains/terrains.hpp"

#include "shadows.hpp"

namespace flounder
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

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderModel(model *object, matrix4x4 *modelMatrix);

		void endRendering();
	public:
		Fbo *getFbo() const { return m_fbo; }
	};
}
