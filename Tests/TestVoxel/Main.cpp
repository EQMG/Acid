#include <iostream>
#include <Files/Files.hpp>
#include <Helpers/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/PlayerFps.hpp"
#include "Scenes/Scene1.hpp"
#include "Voxels/MaterialVoxel.hpp"
#include "Voxels/VoxelChunk.hpp"

using namespace test;
using namespace acid;

int main(int argc, char **argv)
{
	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);

	// Registers file search paths.
	Files::Get()->AddSearchPath("Resources/Engine");

	// Registers modules.

	// Registers components.
	auto &componentRegister = Scenes::Get()->GetComponentRegister();
	componentRegister.Add<PlayerFps>("PlayerFps");
	componentRegister.Add<MaterialVoxel>("MaterialVoxel");
	componentRegister.Add<VoxelChunk>("VoxelChunk");

	// Sets values to modules.
	Window::Get()->SetTitle("Test Voxel");
	Window::Get()->SetIcons({"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png",
		"Icons/Icon-64.png", "Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"});
	Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}
