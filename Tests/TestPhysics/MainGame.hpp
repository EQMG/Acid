#pragma once

#include <Engine/Game.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include "Configs/ConfigManager.hpp"

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
	std::unique_ptr<ConfigManager> m_configs;

	ButtonKeyboard m_buttonFullscreen;
	ButtonKeyboard m_buttonScreenshot;
	ButtonKeyboard m_buttonExit;
};
}
