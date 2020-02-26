#pragma once

#include <Engine/App.hpp>

using namespace acid;

namespace test {
class MainApp : public App {
public:
	MainApp();

	~MainApp();

	void Start() override;
	void Update() override;
};
}
