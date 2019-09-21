#pragma once

#include <Engine/App.hpp>
#include <Files/FileObserver.hpp>
#include <Inputs/ButtonKeyboard.hpp>

using namespace acid;

namespace test {
class MainApp : public App {
public:
	MainApp();

	~MainApp();

	void Start() override;
	void Update() override;

private:
	FileObserver m_fileObserver;

	ButtonKeyboard m_buttonFullscreen;
	ButtonKeyboard m_buttonScreenshot;
	ButtonKeyboard m_buttonExit;
};
}
