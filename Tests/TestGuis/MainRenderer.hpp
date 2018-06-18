#pragma once

#include <Renderer/IManagerRender.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>

using namespace fl;

namespace test
{
	class MainRenderer :
		public IManagerRender
	{
	public:
		MainRenderer();

		~MainRenderer();

		void Update() override;
	};
}
