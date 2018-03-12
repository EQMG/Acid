#include <iostream>
#include <Devices/Mouse.hpp>
#include <Engine/ModuleUpdater.hpp>
#include <Helpers/FileSystem.hpp>
#include <Renderer/Renderer.hpp>
#include <Skyboxes/SkyboxRender.hpp>
#include <Scenes/Scenes.hpp>
#include <Objects/ComponentRegister.hpp>
#include <Scenes/FpsPlayer.hpp>
#include "Configs/ConfigManager.hpp"
#include "Scenes/Scene1.hpp"
#include "ManagerRender.hpp"

using namespace Demo;

//#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//#else
int main(int argc, char **argv)
//#endif
{
	// Creates the engine object.
	auto m_engine = new Engine();
	m_engine->SetUpdater(new ModuleUpdater());

	auto configManager = new ConfigManager();
	printf("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Adds to the component registry.
	ComponentRegister::Register("FpsPlayer", REGISTER_CREATE(FpsPlayer));

	// Registers modules.


	// Initializes the engine modules.
	Display::Get()->SetTitle("Example Starting");
	Display::Get()->SetIcon("Resources/Logos/Tail.png");

	Mouse::Get()->SetCustomMouse("Resources/Guis/Cursor.png");

	Renderer::Get()->SetManager(new ManagerRender());

	Scenes::Get()->SetScene(new Scene1());

	// Runs the engine loop.
	const int exitCode = m_engine->Run();

	// Deletes the engine.
	delete configManager;
	delete m_engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
