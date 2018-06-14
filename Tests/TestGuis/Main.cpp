#include <iostream>
#include <Files/Json/FileJson.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Worlds/Worlds.hpp>
#include "MainUpdater.hpp"
#include "MainRenderer.hpp"
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

	// Registers modules.

	// Registers components.

	// Initializes modules.
	Display::Get()->SetTitle("Testing Guis");
	Display::Get()->SetIcon("Logos/Tail.png");
	Mouse::Get()->SetCustomMouse("Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());
	Worlds::Get()->SetWorld(new MainWorld());

	// Runs the game loop.
	const int exitCode = engine->Run();

	// Deletes the engine.
	delete engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
