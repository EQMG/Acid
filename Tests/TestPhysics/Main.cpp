#include <iostream>
#include <Files/Files.hpp>
#include <Helpers/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Files/FileWatcher.hpp>
#include <Scenes/Scenes.hpp>
#include "Behaviours/HeightDespawn.hpp"
#include "Behaviours/NameTag.hpp"
#include "Configs/ConfigManager.hpp"
#include "MainRenderer.hpp"
#include "Scenes/PlayerFps.hpp"
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
	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);

	// Registers file search paths.
	for (auto &file : FileSystem::FilesInPath(FileSystem::GetWorkingDirectory(), false))
	{
		if (String::Contains(file, "data-"))
		{
			Files::Get()->AddSearchPath(String::ReplaceFirst(file, FileSystem::GetWorkingDirectory() + FileSystem::Separator, ""));
		}
	}

	Files::Get()->AddSearchPath("Resources/Engine");

	// Loads configs from a config manager.
	auto configManager = ConfigManager();
	Log::Out("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Watches all files in the working directory.
	FileWatcher watcher = FileWatcher(FileSystem::GetWorkingDirectory(), Time::Seconds(2.0f));
	watcher.GetOnChange() += [](std::string path, FileWatcher::Status status) {
		switch (status)
		{
		case FileWatcher::Status::Created:
			Log::Out("Created '%s'\n", path.c_str());
			break;
		case FileWatcher::Status::Modified:
			Log::Out("Modified '%s'\n", path.c_str());
			break;
		case FileWatcher::Status::Erased:
			Log::Out("Erased '%s'\n", path.c_str());
			break;
		}
	};

	// Registers modules.
	auto &moduleManager = Engine::Get()->GetModuleManager();
	moduleManager.Add<World>(Module::Stage::Always);
//	moduleManager.Remove<Shadows>();

	// Registers components.
	auto &componentRegister = Scenes::Get()->GetComponentRegister();
	componentRegister.Add<HeightDespawn>("HeightDespawn");
	componentRegister.Add<NameTag>("NameTag");
	componentRegister.Add<PlayerFps>("PlayerFps");
	componentRegister.Add<CelestialBody>("CelestialBody");
	componentRegister.Add<SkyboxCycle>("SkyboxCycle");
	componentRegister.Add<MaterialTerrain>("MaterialTerrain");
	componentRegister.Add<Terrain>("Terrain");

	// Sets values to modules.
	Window::Get()->SetTitle("Test Physics");
	Window::Get()->SetIcon("Logos/Acid_01.png");
	Mouse::Get()->SetCustomMouse("Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	int32_t exitCode = engine->Run();
	configManager.Save();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}
