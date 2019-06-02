#include "MainGame.hpp"

#include <iostream>
#include <Files/Files.hpp>
#include <Files/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Graphics/Graphics.hpp>
#include <Resources/Resources.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"

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
	m_fileWatcher(std::filesystem::current_path(), 2s),
	m_buttonFullscreen(Key::F11),
	m_buttonScreenshot(Key::F9),
	m_buttonExit(Key::Delete)
{
	// Registers file search paths.
	Files::Get()->AddSearchPath("Resources/Engine");
	Log::Out("Working Directory: %s\n", std::filesystem::current_path().c_str());

	// Watches all files in the working directory.
	m_fileWatcher.OnChange().Add([this](std::string path, FileWatcher::Status status)
	{
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
	});

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
				Graphics::Get()->CaptureScreenshot("Screenshots/" + Time::GetDateTime() + ".png");
			});
		}
	});
	m_buttonExit.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Engine::Get()->RequestClose();
		}
	});

	// Registers modules.

	// Registers components.
	auto &componentRegister = Scenes::Get()->GetComponentRegister();

	// Sets values to modules.
	Window::Get()->SetTitle("Test GUI");
	Window::Get()->SetIcons({ "Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png", 
		"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png" });
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	Graphics::Get()->SetRenderer(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());
}

MainGame::~MainGame()
{
	Files::Get()->ClearSearchPath();

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
}

void MainGame::Update()
{
}
}
