#pragma once

#include "../renderer/IRenderer.hpp"

namespace Flounder
{
	class RendererTest :
		public IRenderer
	{
	private:
		Shader *m_shader;
	public:
		RendererTest();

		~RendererTest();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
