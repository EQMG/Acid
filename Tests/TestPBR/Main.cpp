#include <iostream>
#include <Devices/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "Scenes/PlayerFps.hpp"
#include "Scenes/Scene1.hpp"
#include "MainRenderer.hpp"

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

	// Sets values to modules.
	Window::Get()->SetTitle("Test PBR");
	Window::Get()->SetIcon("Logos/Flask.png");
//	Mouse::Get()->SetCustomMouse("Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}
