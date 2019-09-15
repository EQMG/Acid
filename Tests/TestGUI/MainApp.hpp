#pragma once

#include <Engine/App.hpp>
#include <Files/FileWatcher.hpp>
#include <Inputs/ButtonKeyboard.hpp>

using namespace acid;

namespace test {
class MainApp : public App {
public:
	MainApp();

	~MainApp();

	void Update() override;

private:
	FileWatcher m_fileWatcher;

	ButtonKeyboard m_buttonFullscreen;
	ButtonKeyboard m_buttonScreenshot;
	ButtonKeyboard m_buttonExit;
};
}
