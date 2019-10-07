#pragma once

#include <Engine/App.hpp>
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
	ConfigManager m_configManager;
};
}
