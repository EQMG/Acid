#include <iostream>

#include <flounder.hpp>

#include "maincamera.hpp"
#include "mainplayer.hpp"
#include "manageruis.hpp"
#include "instance.hpp"
#include "managerrender.hpp"

#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
#else
int main()
#endif
{
	// Creates the engine object.
	Engine *m_engine = new Engine();
	m_engine->SetUpdater(new GlfwUpdater());

	// Initializes the engine modules.
	if (Display::get() != nullptr)
	{
		Display::get()->setWindowSize(1080, 720);
		Display::get()->setTitle("Flounder Demo");
		Display::get()->setIcon("res/flounder.png");
		Display::get()->setFpsLimit(0);
		Display::get()->setAntialiasing(true);
		Display::get()->setFullscreen(false);
	}

	if (Mouse::Get() != nullptr)
	{
		Mouse::Get()->setCustomMouse("res/cursor.png");
	}

	if (Camera::Get() != nullptr)
	{
		Camera::Get()->SetCamera(new maincamera());
		Camera::Get()->SetPlayer(new mainplayer());
	}

	if (renderer::get() != nullptr)
	{
		renderer::get()->setManager(new managerrender());
	}

	if (uis::get() != nullptr)
	{
		uis::get()->setMaster(new manageruis());
	}

	if (standards::get() != nullptr)
	{
		standards::get()->addStandard(new Instance());
	}

	if (skyboxes::get() != nullptr)
	{
		skyboxes::get()->setSkybox(new skybox(
			new texture(6, "res/skybox/starsRight.png", "res/skybox/starsLeft.png", "res/skybox/starsTop.png", "res/skybox/starsBottom.png", "res/skybox/starsBack.png", "res/skybox/starsFront.png"),
			new model("res/skybox/skyboxSphere.obj"),
			2048.0f
		));
	}

	if (waters::get() != nullptr)
	{
		waters::get()->setWater(new water(
			vector3(), vector3()
		));
	}

	// Runs the engine loop.
	m_engine->Run();

	// Deletes the engine.
	delete m_engine;

	// Pauses the console.
	std::cin.get();
	return EXIT_SUCCESS;
}
