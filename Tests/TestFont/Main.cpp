#include <iostream>
#include <Files/Json/FileJson.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"

using namespace test;
using namespace acid;

int main(int argc, char **argv)
{
	// Registers file search paths.
	Files::SetBaseDirectory(argv[0]);
	Files::AddSearchPath("Resources/Engine");

	// Creates the engine.
	auto engine = std::make_unique<Engine>();

	// Registers modules.

	// Registers components.

	// Initializes modules.
	Display::Get()->SetTitle("Test Font");
	Display::Get()->SetIcon("Logos/Flask.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}
