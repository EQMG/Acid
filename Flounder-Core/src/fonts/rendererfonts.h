#pragma once

#include "../loaders/loaders.h"
#include "../renderer/irenderer.h"
#include "../uis/uis.h"

#include "text.h"

namespace flounder
{
	class rendererfonts :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		rendererfonts();

		~rendererfonts();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderText(text *object);

		void endRendering();
	};
}
