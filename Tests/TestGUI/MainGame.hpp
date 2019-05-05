#pragma once

#include <Engine/Game.hpp>
#include <Files/FileWatcher.hpp>
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

	void Update() override;

private:
	FileWatcher m_fileWatcher;

	ButtonKeyboard m_buttonFullscreen;
	ButtonKeyboard m_buttonScreenshot;
	ButtonKeyboard m_buttonExit;
};
}
