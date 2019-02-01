#include "MainGame.hpp"

#include <Files/Files.hpp>
#include <Files/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"

/*#include "../Editor/cr.h"

static unsigned int CR_STATE version = 1;

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation)
{
	assert(ctx);

	if (ctx->version < version)
	{
		Log::Error("A rollback happened due to failure: %x!\n", ctx->failure);
	}

	version = ctx->version;
	Log::Out("Loaded version: %d\n", ctx->version);

	switch (operation) {
	case CR_LOAD:
	//	Engine::Get()->SetGame(new test::MainGame());
		printf("Loaded!\n");
		return 0;
	case CR_UNLOAD:
	//	Engine::Get()->SetGame(nullptr);
		printf("Unloaded!\n");
		return 0;
	}

	return 0;
}*/

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

namespace test
{
	MainGame::MainGame()
	{
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
		Renderer::Get()->SetManager(new MainRenderer());
		Scenes::Get()->SetScene(new Scene1());
	}

	MainGame::~MainGame()
	{
		Files::Get()->ClearSearchPath();

		Renderer::Get()->SetManager(nullptr);
		Scenes::Get()->SetScene(nullptr);
	}

	void MainGame::Start()
	{
	}

	void MainGame::Update()
	{
	}
}
