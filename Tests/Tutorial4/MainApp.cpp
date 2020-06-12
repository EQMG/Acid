#include "MainApp.hpp"

#include <Files/Files.hpp>
#include <Devices/Mouse.hpp>
#include <Inputs/Input.hpp>
#include <Graphics/Graphics.hpp>
#include <Resources/Resources.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"

int main(int argc, char **argv) {
	using namespace test;

	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);
	engine->SetApp(std::make_unique<MainApp>());

	// Runs the game loop.
	auto exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}

namespace test {
MainApp::MainApp() :
	App("Tutorial4", {1, 0, 0}) {
	// Registers file search paths.
	Log::Out("Working Directory: ", std::filesystem::current_path(), '\n');
	Files::Get()->AddSearchPath("Resources/Engine");

	// Loads a input scheme for this app.
	Input::Get()->AddScheme("Default", std::make_unique<InputScheme>("InputSchemes/DefaultPBR.json"), true);

	Input::Get()->GetButton("fullscreen")->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press)
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
	});
	Input::Get()->GetButton("screenshot")->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Resources::Get()->GetThreadPool().Enqueue([]() {
				Graphics::Get()->CaptureScreenshot(Time::GetDateTime("Screenshots/%Y%m%d%H%M%S.png"));
			});
		}
	});
	Input::Get()->GetButton("exit")->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press)
			Engine::Get()->RequestClose();
	});
}

MainApp::~MainApp() {
	Files::Get()->ClearSearchPath();

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
}

void MainApp::Start() {
	// Sets values to modules.
	Window::Get()->SetTitle("Tutorial4");
	Window::Get()->SetIcons({
		"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png",
		"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
	Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
}

void MainApp::Update() {
}
}
