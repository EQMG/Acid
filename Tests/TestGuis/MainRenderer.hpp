#pragma once

#include <Renderer/IManagerRender.hpp>

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
