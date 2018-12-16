#pragma once

#include <Renderer/RenderManager.hpp>

using namespace acid;

namespace test
{
	class MainRenderer :
		public RenderManager
	{
	public:
		MainRenderer();

		void Start() override;

		void Update() override;
	};
}
