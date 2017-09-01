#pragma once

#include "../renderer/IRenderer.hpp"
#include "../uis/Uis.hpp"

#include "Text.hpp"

namespace Flounder
{
	class RendererFonts :
		public IRenderer
	{
	private:
		shader *m_shader;
	public:
		RendererFonts();

		~RendererFonts();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void RenderText(Text *object);

		void EndRendering();
	};
}
