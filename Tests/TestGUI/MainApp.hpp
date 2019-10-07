#pragma once

#include <Engine/App.hpp>
#include <Files/FileObserver.hpp>

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
};
}
