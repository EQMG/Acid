#pragma once

#include "../renderer/irenderer.hpp"

namespace Flounder
{
	class renderertest :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		renderertest();

		~renderertest();

		void render(const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
