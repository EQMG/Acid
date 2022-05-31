#include "MainApp.hpp"

#include <Files/Files.hpp>
#include <Inputs/Inputs.hpp>
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
	engine = nullptr;

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}

namespace test {
MainApp::MainApp() :
	App("Test Physics", {1, 0, 0}),
	cursor("Guis/Cursor.png", CursorHotspot::UpperLeft)  {
	// Registers file search paths.
#ifdef ACID_PACKED_RESOURCES
	for (auto &file : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (String::StartsWith(file.path().string(), "data-"))
			Files::Get()->AddSearchPath(file.path().string());
	}
#else
	Files::Get()->AddSearchPath("Resources/Engine");
#endif

	// Sets values to modules.
	auto window0 = Windows::Get()->AddWindow();
	window0->SetTitle("Test Physics");
	window0->SetIcons({
		"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png", "Icons/Icon-96.png",
		"Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
	window0->SetCursor(&cursor);
	//auto window1 = Windows::Get()->AddWindow();
	//window1->SetTitle("Test Physics 2");

	// Loads a input scheme for this app.
	Inputs::Get()->AddScheme("Default", std::make_unique<InputScheme>("InputSchemes/DefaultPhysics.json"), true);

	Inputs::Get()->GetButton("fullscreen")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press)
			Windows::Get()->GetWindow(0)->SetFullscreen(!Windows::Get()->GetWindow(0)->IsFullscreen());
	});
	Inputs::Get()->GetButton("screenshot")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			Resources::Get()->GetThreadPool().Enqueue([]() {
				Graphics::Get()->CaptureScreenshot(Time::GetDateTime("Screenshots/%Y%m%d%H%M%S.png"));
			});
		}
	});
	Inputs::Get()->GetButton("exit")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press)
			Engine::Get()->RequestClose();
	});
}

MainApp::~MainApp() {
	configManager.Save();
	// TODO: Only clear our search paths (leave Engine resources alone!)
	Files::Get()->ClearSearchPath();

//	Graphics::Get()->SetRenderer(nullptr);
//	Scenes::Get()->SetScene(nullptr);
}

void MainApp::Start() {
	configManager.Load();
	Log::Out("Current DateTime: ", Time::GetDateTime(), '\n');
	Log::Out("Working Directory: ", std::filesystem::current_path(), '\n');

	Timers::Get()->Once([]() {
		Log::Out("Timer Hello World!\n");
	}, 0.333s);
	Timers::Get()->Every([]() {
		Log::Out(Engine::Get()->GetFps(), " fps, ", Engine::Get()->GetUps(), " ups\n");
	}, 3s);
	Timers::Get()->Repeat([]() {
		static uint32_t i = 0;
		Log::Out("Timer Repeat Tick #", ++i, '\n');
	}, 2s, 3);

	// Sets values to modules.
	Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
	Scenes::Get()->SetScene(std::make_unique<Scene1>());
}

void MainApp::Update() {
}
}
