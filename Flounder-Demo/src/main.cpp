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
	// Creates the framework object.
	framework *m_framework = new framework();
	m_framework->setUpdater(new glfwupdater());

	// Initializes the framework modules.
	if (display::get() != nullptr)
	{
		display::get()->setWindowSize(1080, 720);
		display::get()->setTitle("Flounder Demo");
		display::get()->setIcon("res/flounder.png");
		display::get()->setFpsLimit(0);
		display::get()->setAntialiasing(true);
		display::get()->setFullscreen(false);
	}

	if (mouse::get() != nullptr)
	{
		mouse::get()->setCustomMouse("res/cursor.png");
	}

	if (camera::get() != nullptr)
	{
		camera::get()->setCamera(new maincamera());
		camera::get()->setPlayer(new mainplayer());
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
		standards::get()->addStandard(new instance());
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

	// Runs the framework loop.
	m_framework->run();

	// Deletes the framework.
	delete m_framework;

	// Pauses the console.
	std::cin.get();
	return EXIT_SUCCESS;
}
