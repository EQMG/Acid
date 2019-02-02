#include "MainGame.hpp"

#include <Files/Files.hpp>
#include <Files/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"

#if defined(ACID_RELOAD)
#include <Engine/cr.h>

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation)
{
	Log::Out("[Guest] Engine instance: %p\n", (void *)Engine::Get());

	if (Engine::Get() != nullptr)
	{
		Log::Out("[Guest] Game instance: %p\n", (void *)Engine::Get()->GetGame());
	}

	switch (operation) {
	case CR_LOAD:
		Log::Out("[Guest] Operation load, %i\n", ctx->version);
		if (Engine::Get()->GetGame() == nullptr)
		{
			Engine::Get()->SetGame(new test::MainGame());
		}
		return 0;
	case CR_UNLOAD:
		Log::Out("[Guest] Operation unload, %i\n", ctx->version);
		return 0;
	}

	return 0;
}
#else
int main(int argc, char **argv)
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
}
#endif

namespace test
{
	MainGame::MainGame()
	{
		Log::Out("[Game] Constructor\n");
		// Registers file search paths.
		Files::Get()->AddSearchPath("Resources/Engine");
		Log::Out("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

		// Registers modules.
		auto &moduleManager = Engine::Get()->GetModuleManager();

		// Registers components.
		auto &componentRegister = Scenes::Get()->GetComponentRegister();

		// Sets values to modules.
		Window::Get()->SetTitle("Test GUI");
		Window::Get()->SetIcons({"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png",
			"Icons/Icon-64.png", "Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"});
	//	Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
		if (Renderer::Get()->GetManager() == nullptr)
		{
			Renderer::Get()->SetManager(new MainRenderer());
		}
		if (Scenes::Get()->GetScene() == nullptr)
		{
			Scenes::Get()->SetScene(new Scene1());
		}
	}

	MainGame::~MainGame()
	{
		Log::Out("[Game] Destructor\n");
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
