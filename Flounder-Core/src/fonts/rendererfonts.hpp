#pragma once

#include "../renderer/irenderer.hpp"
#include "../uis/uis.hpp"

#include "text.hpp"

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
