#pragma once

#include <Engine/App.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include "Configs/ConfigManager.hpp"

using namespace acid;

namespace test {
class MainApp : public App, public virtual Observer {
public:
	MainApp();

	~MainApp();

	void Start() override;
	void Update() override;

private:
	ConfigManager m_configs;

	ButtonKeyboard m_buttonFullscreen;
	ButtonKeyboard m_buttonScreenshot;
	ButtonKeyboard m_buttonExit;
};
}
