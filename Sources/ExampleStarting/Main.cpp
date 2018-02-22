#include <iostream>
#include <Camera/Camera.hpp>
#include <Devices/Mouse.hpp>
#include <Engine/ModuleUpdater.hpp>
#include <Helpers/FileSystem.hpp>
#include <Meshes/Mesh.hpp>
#include <Renderer/Renderer.hpp>
#include <Skyboxes/SkyboxRender.hpp>
#include <Scenes/Scenes.hpp>
#include "Configs/ConfigManager.hpp"
#include "Cameras/FpsCamera.hpp"
#include "Cameras/FpsPlayer.hpp"
#include "Scenes/Scene1.hpp"
#include "Renderers/ManagerRender.hpp"
#include "Uis/ManagerUis.hpp"

using namespace Demo;

//#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//#else
int main(int argc, char **argv)
//#endif
{
	// Creates the engine object.
	Engine *m_engine = new Engine();
	m_engine->SetUpdater(new ModuleUpdater());

	printf("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());
	ConfigManager *configManager = new ConfigManager();

	// Initializes the engine modules.
	if (Display::Get() != nullptr)
	{
		Display::Get()->SetTitle("Example Starting");
		Display::Get()->SetIcon("Resources/Logos/Tail.png");
	}

	if (Mouse::Get() != nullptr)
	{
		Mouse::Get()->SetCustomMouse("Resources/Guis/Cursor.png");
	}

	if (Camera::Get() != nullptr)
	{
		Camera::Get()->SetCamera(new FpsCamera());
		Camera::Get()->SetPlayer(new FpsPlayer());
	}

	if (Renderer::Get() != nullptr)
	{
		Renderer::Get()->SetManager(new ManagerRender());
	}

	if (Uis::Get() != nullptr)
	{
		Uis::Get()->SetManager(new ManagerUis());
	}

	if (Scenes::Get() != nullptr)
	{
		Scenes::Get()->SetScene(new Scene1());
	}

	// Runs the engine loop.
	const int exitCode = m_engine->Run();

	// Deletes the engine.
	delete configManager;
	delete m_engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
