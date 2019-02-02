#pragma once

#include <Engine/Game.hpp>

using namespace acid;

namespace test
{
	class MainGame :
		public Game
	{
	public:
		MainGame();

		~MainGame();

		void Start() override;

		void Update() override;
	};
}