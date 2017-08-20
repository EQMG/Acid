#pragma once

#include "../renderer/irenderer.h"

namespace flounder
{
	class renderertest :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		renderertest();

		~renderertest();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	};
}
