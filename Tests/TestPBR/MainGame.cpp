#include "MainGame.hpp"

#include <iostream>
#include <thread>
#include <Files/Files.hpp>
#include <Files/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "Scenes/PlayerFps.hpp"
#include "Scenes/Scene1.hpp"
#include "MainRenderer.hpp"

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
		m_buttonScreenshot(Key::F12),
		m_buttonExit(Key::Delete)
	{
		// Registers file search paths.
		Files::Get()->AddSearchPath("Resources/Engine");
		Log::Out("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

		// Registers modules.
		auto &moduleManager = Engine::Get()->GetModuleManager();

		// Registers components.
		auto &componentRegister = Scenes::Get()->GetComponentRegister();
		componentRegister.Add<PlayerFps>("PlayerFps");

		// Sets values to modules.
		Window::Get()->SetTitle("Test PBR");
		Window::Get()->SetIcons({
			"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png",
			"Icons/Icon-64.png", "Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
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
		if (m_buttonFullscreen.WasDown())
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}

		if (m_buttonScreenshot.WasDown())
		{
			// TODO: Threading.
			std::thread t([]()
			{
				std::string filename = "Screenshots/" + Engine::GetDateTime() + ".png";
				Renderer::Get()->CaptureScreenshot(filename);
			});
			t.detach();
		}

		if (m_buttonExit.WasDown())
		{
			Engine::Get()->RequestClose(false);
		}
	}

	void MainGame::Update()
	{
	}
}
