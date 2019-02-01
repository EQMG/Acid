#include "MainGame.hpp"

#include <Files/Files.hpp>
#include <Files/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"

#include <Engine/cr.h>

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation)
{
	Log::Out("[Game_Main] Engine instance: %i\n", Engine::Get() != nullptr);

	if (Engine::Get() != nullptr)
	{
		Log::Out("[Game_Main] Game instance: %i\n", Engine::Get()->GetGame() != nullptr);
	}

	switch (operation) {
	case CR_LOAD:
		Log::Out("[Game_Main] Loaded!\n");
	//	Engine::Get()->SetGame(nullptr);
		if (Engine::Get()->GetGame() == nullptr)
		{
			Engine::Get()->SetGame(new test::MainGame());
		}
		return 0;
	case CR_UNLOAD:
		Log::Out("[Game_Main] Unloaded!\n");
	//	delete dynamic_cast<test::MainGame *>(Engine::Get()->GetGame());
	//	Engine::Get()->SetGame(nullptr);
		return 0;
	}

	return 0;
}

/*int main(int argc, char **argv)
{
	using namespace test;

	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);
	engine->SetGame(new MainGame());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}*/

namespace test
{
	MainGame::MainGame()
	{
		Log::Out("[Game] Constructor!\n");
		// Registers file search paths.
		Files::Get()->AddSearchPath("Resources/Engine");
		Log::Out("[Game] Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

		// Registers modules.
		auto &moduleManager = Engine::Get()->GetModuleManager();

		// Registers components.
		auto &componentRegister = Scenes::Get()->GetComponentRegister();

		// Sets values to modules.
		Window::Get()->SetTitle("Test GUI");
		Window::Get()->SetIcons({"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png",
			"Icons/Icon-64.png", "Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"});
	//	Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
		Renderer::Get()->SetManager(new MainRenderer());
		Scenes::Get()->SetScene(new Scene1());
	}

	MainGame::~MainGame()
	{
		Log::Out("[Game] Destructor!\n");
	//	Files::Get()->ClearSearchPath();

	//	Uis::Get()->GetContainer()->ClearChildren();
	//	Renderer::Get()->SetManager(nullptr);
	//	Scenes::Get()->SetScene(nullptr);
	}

	void MainGame::Start()
	{
	}

	void MainGame::Update()
	{
	}
}
