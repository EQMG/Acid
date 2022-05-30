#include "MainApp.hpp"

#ifdef ACID_RELOAD
#include <cr.h>

#include <Devices/Mouse.hpp>
#include <Inputs/Inputs.hpp>
#include <Files/Files.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"
#include "Resources/Resources.hpp"

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation) {
	switch (operation) {
	case CR_LOAD:
		Log::Debug("[Guest] Operation load: ", ctx->version, '\n');
		Engine::Get()->SetApp(std::make_unique<test::MainApp>());
		return 0;
	case CR_UNLOAD:
		Log::Debug("[Guest] Operation unload: ", ctx->version, '\n');
		Engine::Get()->SetApp(nullptr);
		return 0;
	default:
		return 0;
	}
}
#else
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
#endif

namespace test {
MainApp::MainApp() :
	App("Editor Test", {1, 0, 0}) {
	Log::Debug("[Game] Constructor\n");

	// Registers file search paths.
	Log::Out("Working Directory: ", std::filesystem::current_path(), '\n');
	Files::Get()->AddSearchPath("Resources/Engine");

	// Loads a input scheme for this app.
	Inputs::Get()->AddScheme("Default", std::make_unique<InputScheme>("InputSchemes/DefaultPhysics.json"), true);

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
	Log::Debug("[Game] Destructor\n");
	//Files::Get()->ClearSearchPath();

	Inputs::Get()->RemoveScheme("Default");

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
	Uis::Get()->GetCanvas().ClearChildren();
}

void MainApp::Start() {
	// Sets values to modules.
#ifndef ACID_RELOAD
	Windows::Get()->GetWindow(0)->SetTitle("Test Physics");
	Windows::Get()->GetWindow(0)->SetIcons({
		"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png",
		"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
#endif
	Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
	Scenes::Get()->SetScene(std::make_unique<Scene1>());
}

void MainApp::Update() {
}
}
