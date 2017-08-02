#pragma once

#include "../deferred/rendererdeferred.h"
#include "../fbos/fbo.h"
#include "../renderer/irenderer.h"

#include "shadows.h"

namespace flounder
{
	class renderershadows :
		public irenderer
	{
	private:
		fbo *m_fbo;
		shader *m_shader;
	public:
		renderershadows();

		~renderershadows();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

	//	void renderEntity(entity *object);

		void endRendering();
	public:
		fbo *getFbo() const { return m_fbo; }
	};
}
