#pragma once

#include <Engine/App.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include "Configs/ConfigManager.hpp"

using namespace acid;

namespace test {
class MainApp :
	public App,
	public Observer {
public:
	MainApp();

	~MainApp();

	void Update() override;

private:
	std::unique_ptr<ConfigManager> m_configs;

	ButtonKeyboard m_buttonFullscreen;
	ButtonKeyboard m_buttonScreenshot;
	ButtonKeyboard m_buttonExit;
};
}
