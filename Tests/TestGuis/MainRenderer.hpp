#pragma once

#include <Renderer/IManagerRender.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>

using namespace acid;

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
