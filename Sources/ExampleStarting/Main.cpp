#include <iostream>
#include <Inputs/Mouse.hpp>
#include <Files/Json/FileJson.hpp>
#include <Helpers/FileSystem.hpp>
#include <Renderer/Renderer.hpp>
#include <Skyboxes/SkyboxRender.hpp>
#include <Scenes/Scenes.hpp>
#include <Terrains/LodBehaviour.hpp>
#include "Scenes/FpsPlayer.hpp"
#include "Configs/ConfigManager.hpp"
#include "Scenes/Scene1.hpp"
#include "ManagerRender.hpp"

using namespace Demo;

//#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//#else
int main(int argc, char **argv)
//#endif
{
	// Creates the engine object.
	auto engine = new Engine();

	auto configManager = new ConfigManager();
	printf("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Registers modules.
//	Engine::Get()->RegisterModule<Example>("Example");

	// Registers components.
	Scenes::Get()->RegisterComponent<FpsPlayer>("FpsPlayer");
	Scenes::Get()->RegisterComponent<LodBehaviour>("LodBehaviour");

	// Initializes modules.
	Display::Get()->SetTitle("Example Starting");
	Display::Get()->SetIcon("Resources/Logos/Tail.png");
	Mouse::Get()->SetCustomMouse("Resources/Guis/Cursor.png");
	Renderer::Get()->SetManager(new ManagerRender());
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
