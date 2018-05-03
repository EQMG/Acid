#include <iostream>
#include <Files/Json/FileJson.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Terrains/LodBehaviour.hpp>
#include "Configs/ConfigManager.hpp"
#include "MainUpdater.hpp"
#include "MainRenderer.hpp"
#include "Scenes/FpsPlayer.hpp"
#include "Scenes/Scene1.hpp"

using namespace Demo;
using namespace fl;

//#if (FL_BUILD_RELEASE && FL_BUILD_WINDOWS)
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//#else
int main(int argc, char **argv)
//#endif
{
	// Creates the engine and updater objects.
	auto engine = new Engine();
	engine->SetUpdater(new MainUpdater());

	auto configManager = new ConfigManager();
	printf("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Registers modules.
//	Engine::Get()->RegisterModule<Example>("Example");
//	Engine::Get()->DeregisterModule("shadows");

	// Registers components.
	Scenes::Get()->RegisterComponent<FpsPlayer>("FpsPlayer");
	Scenes::Get()->RegisterComponent<LodBehaviour>("LodBehaviour");

	// Initializes modules.
	Display::Get()->SetTitle("Example Starting");
	Display::Get()->SetIcon("Resources/Logos/Tail.png");
	Mouse::Get()->SetCustomMouse("Resources/Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	const int exitCode = engine->Run();

	// Deletes the engine.
	delete configManager;
	delete engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
