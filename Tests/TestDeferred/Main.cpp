#include <iostream>
#include <Animations/MaterialAnimated.hpp>
#include <Animations/MeshAnimated.hpp>
#include <Files/Files.hpp>
#include <Files/Json/FileJson.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "World/World.hpp"
#include "Skybox/CelestialBody.hpp"
#include "Skybox/SkyboxCycle.hpp"
#include "Configs/ConfigManager.hpp"
#include "MainUpdater.hpp"
#include "MainRenderer.hpp"
#include "Scenes/FpsPlayer.hpp"
#include "Scenes/Scene1.hpp"

using namespace test;
using namespace fl;

//#if (FL_BUILD_RELEASE && FL_BUILD_WINDOWS)
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//#else
int main(int argc, char **argv)
//#endif
{
	// Registers file search paths.
//	Files::AddSearchPath("Resources/Game");
	Files::AddSearchPath("Resources/Engine");

	// Creates the engine and updater objects.
	auto engine = new Engine();
	engine->SetUpdater(new MainUpdater());

//	auto modelAnimated = new MeshAnimated("Objects/Animated/Model.json");
//	return 0;

	auto configManager = new ConfigManager();
	printf("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Registers modules.
//	Engine::Get()->RegisterModule<Example>(UPDATE_NORMAL, "Example");
	Engine::Get()->RegisterModule<World>(UPDATE_NORMAL, "World");
//	Engine::Get()->DeregisterModule("shadows");

	// Registers components.
	Scenes::Get()->RegisterComponent<FpsPlayer>("FpsPlayer");
	Scenes::Get()->RegisterComponent<CelestialBody>("CelestialBody");
	Scenes::Get()->RegisterComponent<SkyboxCycle>("SkyboxCycle");
	Scenes::Get()->RegisterComponent<MaterialAnimated>("MaterialAnimated");

	// Initializes modules.
	Display::Get()->SetTitle("Test Deferred");
	Display::Get()->SetIcon("Logos/Tail.png");
	Mouse::Get()->SetCustomMouse("Guis/Cursor.png");
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
