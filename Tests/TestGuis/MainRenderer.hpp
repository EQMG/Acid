#pragma once

#include <Renderer/IManagerRender.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>

using namespace fl;

namespace Demo
{
	class MainRenderer :
		public IManagerRender
	{
	private:
		Vector4 m_infinity;

		RendererGuis *m_rendererGuis;
		RendererFonts *m_rendererFonts;
	public:
		MainRenderer();

		~MainRenderer();

		void Render() override;
	};
}
