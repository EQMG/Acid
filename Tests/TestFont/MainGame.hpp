#pragma once

#include <Engine/Game.hpp>
#include <Inputs/ButtonKeyboard.hpp>

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

	private:
		ButtonKeyboard m_buttonFullscreen;
		ButtonKeyboard m_buttonScreenshot;
		ButtonKeyboard m_buttonExit;
	};
}
