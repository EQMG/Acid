#include "MainApp.hpp"

#include <Inputs/Inputs.hpp>
#include <Files/Files.hpp>
#include <Graphics/Graphics.hpp>
#include <Resources/Resources.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"

int main(int argc, char **argv) {
	using namespace test;

	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);
	engine->SetApp(std::make_unique<MainApp>());

	// Runs the game loop.
	auto exitCode = engine->Run();
	engine = nullptr;

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}

namespace test {
MainApp::MainApp() :
	App("Test GUI", {1, 0, 0}),
	fileObserver(std::filesystem::current_path(), 2s) {
	// Registers file search paths.
	Log::Out("Working Directory: ", std::filesystem::current_path(), '\n');
	Files::Get()->AddSearchPath("Resources/Engine");

	// Watches all files in the working directory.
	fileObserver.OnChange().connect(this, [this](std::filesystem::path path, FileObserver::Status status) {
		switch (status) {
		case FileObserver::Status::Created:
			Log::Out("Created ", path, '\n');
			break;
		case FileObserver::Status::Modified:
			Log::Out("Modified ", path, '\n');
			break;
		case FileObserver::Status::Erased:
			Log::Out("Erased ", path, '\n');
			break;
		}
	});

	// Loads a input scheme for this app.
	Inputs::Get()->AddScheme("Default", std::make_unique<InputScheme>("InputSchemes/DefaultGUI.json"), true);

	Inputs::Get()->GetButton("fullscreen")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			Windows::Get()->GetWindow(0)->SetFullscreen(!Windows::Get()->GetWindow(0)->IsFullscreen());
		}
	});
	Inputs::Get()->GetButton("screenshot")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			Resources::Get()->GetThreadPool().Enqueue([]() {
				Graphics::Get()->CaptureScreenshot(Time::GetDateTime("Screenshots/%Y%m%d%H%M%S.png"));
			});
		}
	});
	Inputs::Get()->GetButton("exit")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			Engine::Get()->RequestClose();
		}
	});
}

MainApp::~MainApp() {
	Files::Get()->ClearSearchPath();

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
}

void MainApp::Start() {
	// Sets values to modules.
	Windows::Get()->GetWindow(0)->SetTitle("Test GUI");
	Windows::Get()->GetWindow(0)->SetIcons({
		"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png",
		"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
	Scenes::Get()->SetScene(std::make_unique<Scene1>());
}

void MainApp::Update() {
}
}
