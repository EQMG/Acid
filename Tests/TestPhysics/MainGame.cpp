#include "MainGame.hpp"

#include <iostream>
#include <Files/Files.hpp>
#include <Files/FileSystem.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Devices/Mouse.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Timers/Timers.hpp>
#include "Behaviours/HeightDespawn.hpp"
#include "Behaviours/NameTag.hpp"
#include "MainRenderer.hpp"
#include "Scenes/PlayerFps.hpp"
#include "Scenes/Scene1.hpp"
#include "Skybox/CelestialBody.hpp"
#include "Skybox/SkyboxCycle.hpp"
#include "Terrain/MaterialTerrain.hpp"
#include "Terrain/Terrain.hpp"
#include "World/World.hpp"
#include "Resources/Resources.hpp"

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
MainGame::MainGame() :
	m_buttonFullscreen(Key::F11),
	m_buttonScreenshot(Key::F9),
	m_buttonExit(Key::Delete)
{
	/*Timers::Get()->Repeat(Time::Seconds(4.0f), 5, []()
	{
		static uint32_t i = 0;
		Log::Out("Timer Repeat Tick #%i\n", i);
		i++;
	}, this);
	Timers::Get()->Every(Time::Seconds(2.0f), []()
	{
		Log::Out("Timer Every Tick\n");
	}, this);*/

	// Registers file search paths.
	for (auto &file : FileSystem::FilesInPath(FileSystem::GetWorkingDirectory(), false))
	{
		if (String::Contains(file, "data-"))
		{
			Files::Get()->AddSearchPath(String::ReplaceFirst(file, FileSystem::GetWorkingDirectory() + FileSystem::Separator, ""));
		}
	}

	Files::Get()->AddSearchPath("Resources/Engine");
	Log::Out("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Loads configs from a config manager.
	m_configs = std::make_unique<ConfigManager>();

	m_buttonFullscreen.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}
	});
	m_buttonScreenshot.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Resources::Get()->GetThreadPool().Enqueue([]()
			{
				Graphics::Get()->CaptureScreenshot("Screenshots/" + Engine::GetDateTime() + ".png");
			});
		}
	});
	m_buttonExit.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Engine::Get()->RequestClose(false);
		}
	});

	// Registers modules.
	Engine::Get()->AddModule<World>(Module::Stage::Always);
	//Engine::Get()->RemoveModule<Shadows>();

	auto worldTypeId = GetModuleTypeId<World>();

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
	Window::Get()->SetIcons({ "Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png", "Icons/Icon-96.png",
		"Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png" });
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	Graphics::Get()->SetRenderer(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());
}

MainGame::~MainGame()
{
	m_configs->Save();
	Files::Get()->ClearSearchPath();

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
}

void MainGame::Update()
{
}
}
