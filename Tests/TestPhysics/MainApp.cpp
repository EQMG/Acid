#include "MainApp.hpp"

#include <Files/Files.hpp>
#include <Inputs/Input.hpp>
#include <Devices/Mouse.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Timers/Timers.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"
#include "World/World.hpp"
#include "Resources/Resources.hpp"

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
	App("Test Physics", {1, 0, 0}) {
	// Registers file search paths.
#if defined(ACID_PACKED_RESOURCES)
	for (auto &file : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (String::StartsWith(file.path().string(), "data-"))
			Files::Get()->AddSearchPath(file.path().string());
	}
#else
	Files::Get()->AddSearchPath("Resources/Engine");
#endif

	// Loads a input scheme for this app.
	Input::Get()->AddScheme("Default", std::make_unique<InputScheme>("InputSchemes/DefaultPhysics.json"), true);

	Input::Get()->GetButton("fullscreen")->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}
	}, this);
	Input::Get()->GetButton("screenshot")->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Resources::Get()->GetThreadPool().Enqueue([]() {
				Graphics::Get()->CaptureScreenshot(Time::GetDateTime("Screenshots/%Y%m%d%H%M%S.png"));
			});
		}
	}, this);
	Input::Get()->GetButton("exit")->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Engine::Get()->RequestClose();
		}
	}, this);

	// Registers modules.
	World::Register(Module::Stage::Always);
	//Shadows::Deregister();
}

MainApp::~MainApp() {
	m_configManager.Save();
	// TODO: Only clear our search paths (leave Engine resources alone!)
	Files::Get()->ClearSearchPath();

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
}

void MainApp::Start() {
	m_configManager.Load();
	Log::Out("Current DateTime: ", Time::GetDateTime(), '\n');
	Log::Out("Working Directory: ", std::filesystem::current_path(), '\n');

	Timers::Get()->Once(0.333s, []() {
		Log::Out("Timer Hello World!\n");
	});
	Timers::Get()->Every(3s, []() {
		Log::Out(Engine::Get()->GetFps(), " fps, ", Engine::Get()->GetUps(), " ups\n");
	});
	Timers::Get()->Repeat(2s, 3, []() {
		static uint32_t i = 0;
		Log::Out("Timer Repeat Tick #", i, '\n');
		i++;
	});

	// Sets values to modules.
	Window::Get()->SetTitle("Test Physics");
	Window::Get()->SetIcons({
		"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png", "Icons/Icon-96.png",
		"Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
	Scenes::Get()->SetScene(std::make_unique<Scene1>());
}

void MainApp::Update() {
}
}
