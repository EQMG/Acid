#pragma once

#include "../renderer/irenderer.hpp"

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
