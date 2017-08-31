#pragma once

#include "../renderer/irenderer.hpp"
#include "../uis/uis.hpp"

#include "text.hpp"

namespace Flounder
{
	class rendererfonts :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		rendererfonts();

		~rendererfonts();

		void render(const vector4 &clipPlane, const ICamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const ICamera &camera);

		void renderText(text *object);

		void endRendering();
	};
}
