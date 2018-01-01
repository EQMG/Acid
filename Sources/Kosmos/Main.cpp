#include <iostream>
#include <Platforms/PlatformUpdater.hpp>
#include <Devices/Mouse.hpp>
#include <Camera/Camera.hpp>
#include <Renderer/Renderer.hpp>
#include <Standards/Standards.hpp>
#include <Models/CreateSphere.hpp>
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"
#include "Instance.hpp"
#include "ManagerRender.hpp"
#include "ManagerUis.hpp"

using namespace Demo;

//#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//#else
int main(int argc, char **argv)
//#endif
{
	// Creates the engine object.
	Engine *m_engine = new Engine();
	m_engine->SetUpdater(new PlatformUpdater());

	// Initializes the engine modules.
	if (Display::Get() != nullptr)
	{
		Display::Get()->SetWindowSize(1080, 720);
		Display::Get()->SetTitle("Kosmos");
		Display::Get()->SetIcon("Resources/Logos/Tail.png");
		Display::Get()->SetFpsLimit(0.0f);
		Display::Get()->SetAntialiasing(true);
		Display::Get()->SetFullscreen(false);
	}

	if (Mouse::Get() != nullptr)
	{
		Mouse::Get()->SetCustomMouse("Resources/Cursor.png");
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

	if (Standards::get() != nullptr)
	{
		Standards::get()->AddStandard(new Instance());
	}

	if (Skyboxes::Get() != nullptr)
	{
		Skyboxes::Get()->SetSkybox(new Skybox(
			new Cubemap("Resources/Skyboxes/Stars/Stars", ".png"),
			CreateSphere::Create(6, 6, 1.0f), // new Model("Resources/Skyboxes/SkyboxSphere.obj"),
			2048.0f
		));
	}

	if (Terrains::Get() != nullptr)
	{
		const int n = 5;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				Terrains::Get()->GetTerrains()->push_back(new Terrain(
					Transform(Vector3(2.0f * static_cast<float>(j) * Terrain::SIDE_LENGTH, 0.0f, 2.0f * static_cast<float>(w) * Terrain::SIDE_LENGTH))
				));
			}
		}

		//	Terrains::Get()->GetTerrains()->push_back(new Terrain(Vector3(), Vector3()));
	}

	if (Waters::Get() != nullptr)
	{
		Waters::Get()->SetWater(new Water(
			Vector3(0.0f, 0.0f, 0.0f), Vector3()
		));
	}

	// Runs the engine loop.
	const int exitCode = m_engine->Run();

	// Deletes the engine.
	delete m_engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
