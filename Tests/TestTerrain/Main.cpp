#include <iostream>
#include <Files/Json/FileJson.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Worlds/Worlds.hpp>
#include "Terrains/LodBehaviour.hpp"
#include "Terrains/MaterialTerrain.hpp"
#include "Voxels/MaterialVoxel.hpp"
#include "Waters/MaterialWater.hpp"
#include "Configs/ConfigManager.hpp"
#include "MainUpdater.hpp"
#include "MainRenderer.hpp"
#include "Scenes/FpsPlayer.hpp"
#include "Scenes/Scene1.hpp"
#include "MainWorld.hpp"

using namespace test;
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
	Scenes::Get()->RegisterComponent<MaterialTerrain>("MaterialTerrain");
	Scenes::Get()->RegisterComponent<MaterialVoxel>("MaterialVoxel");
	Scenes::Get()->RegisterComponent<MaterialWater>("MaterialWater");

	// Initializes modules.
	Display::Get()->SetTitle("Test Deferred");
	Display::Get()->SetIcon("Resources/Logos/Tail.png");
	Mouse::Get()->SetCustomMouse("Resources/Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());
	Worlds::Get()->SetWorld(new MainWorld());

	// Runs the game loop.
	const int exitCode = engine->Run();

	// Deletes the engine.
	delete configManager;
	delete engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
