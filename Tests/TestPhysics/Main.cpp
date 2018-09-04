#include <iostream>
#include <Files/Files.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "Configs/ConfigManager.hpp"
#include "MainRenderer.hpp"
#include "Scenes/FpsPlayer.hpp"
#include "Scenes/Scene1.hpp"
#include "Skybox/CelestialBody.hpp"
#include "Skybox/SkyboxCycle.hpp"
#include "Terrain/MaterialTerrain.hpp"
#include "Terrain/Terrain.hpp"
#include "World/World.hpp"

using namespace test;
using namespace acid;

int main(int argc, char **argv)
{
	// Registers file search paths.
//	Files::AddSearchPath("Resources/Game");
	Files::AddSearchPath("Resources/Engine");

	// Creates the engine.
	auto engine = std::make_unique<Engine>();

	auto configManager = std::make_unique<ConfigManager>();
	Log::Out("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Registers modules.
	Engine::Get()->RegisterModule<World>(UPDATE_NORMAL);
//	Engine::Get()->DeregisterModule<Shadows>();

	// Registers components.
	Scenes::Get()->RegisterComponent<FpsPlayer>("FpsPlayer");
	Scenes::Get()->RegisterComponent<CelestialBody>("CelestialBody");
	Scenes::Get()->RegisterComponent<SkyboxCycle>("SkyboxCycle");
	Scenes::Get()->RegisterComponent<MaterialTerrain>("MaterialTerrain");
	Scenes::Get()->RegisterComponent<Terrain>("Terrain");

	// Initializes modules.
	Display::Get()->SetTitle("Test Physics");
	Display::Get()->SetIcon("Logos/Flask.png");
	Mouse::Get()->SetCustomMouse("Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
//	std::cin.get();
	return exitCode;
}
