#include <iostream>

#include <Flounder.hpp>

#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"
#include "ManagerUis.hpp"
#include "Instance.hpp"
#include "ManagerRender.hpp"

#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
#else
int main(int argc, char **argv)
#endif
{
	// Creates the engine object.
	Engine *m_engine = new Engine();
	m_engine->SetUpdater(new GlfwUpdater());

	// Initializes the engine modules.
	if (Display::Get() != nullptr)
	{
		Display::Get()->SetWindowSize(1080, 720);
		Display::Get()->SetTitle("Flounder Demo");
		Display::Get()->SetIcon("res/flounder.png");
		Display::Get()->SetFpsLimit(0); // 61
		Display::Get()->SetAntialiasing(true);
		Display::Get()->SetFullscreen(false);
	}

	//if (Mouse::Get() != nullptr)
	//{
	//	Mouse::Get()->SetCustomMouse("res/cursor.png");
	//}

	if (Camera::Get() != nullptr)
	{
		Camera::Get()->SetCamera(new FpsCamera());
		Camera::Get()->SetPlayer(new FpsPlayer());
	}

	if (Renderer::Get() != nullptr)
	{
		Renderer::Get()->SetManager(new ManagerRender());
	}

	if (Uis::get() != nullptr)
	{
		Uis::get()->SetManager(new ManagerUis());
	}

	if (Standards::get() != nullptr)
	{
		Standards::get()->AddStandard(new Instance());
	}

	/*if (Skyboxes::Get() != nullptr)
	{
		Skyboxes::Get()->SetSkybox(new Skybox(
			new Texture(6, "res/skybox/starsRight.png", "res/skybox/starsLeft.png", "res/skybox/starsTop.png", "res/skybox/starsBottom.png", "res/skybox/starsBack.png", "res/skybox/starsFront.png"),
			new Model("res/skybox/skyboxSphere.obj"),
			2048.0f
		));
	}

	if (Waters::Get() != nullptr)
	{
		Waters::Get()->SetWater(new Water(
			Vector3(), Vector3()
		));
	}*/

	// Runs the engine loop.
	int exitCode = m_engine->Run();

	// Deletes the engine.
	delete m_engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
