#include <iostream>

#include <flounder.h>

#include "maincamera.h"
#include "mainplayer.h"
#include "manageruis.h"
#include "instance.h"
#include "managerrender.h"

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
	display::get()->setWindowSize(1080, 720);
	display::get()->setTitle("Flounder Demo");
	display::get()->setIcon("res/flounder.png");
	display::get()->setFpsLimit(0);
	display::get()->setVSync(false);
	display::get()->setAntialiasing(true);
	display::get()->setSamples(0);
	display::get()->setFullscreen(false);

	mouse::get()->setCustomMouse("res/cursor.png");

	camera::get()->setCamera(new maincamera());
	camera::get()->setPlayer(new mainplayer());
	renderer::get()->setManager(new managerrender());
	uis::get()->setMaster(new manageruis());
	standards::get()->addStandard(new instance());

	skyboxes::get()->setSkybox(new skybox(
		new texture(6, "res/skybox/starsRight.png", "res/skybox/starsLeft.png", "res/skybox/starsTop.png", "res/skybox/starsBottom.png", "res/skybox/starsBack.png", "res/skybox/starsFront.png"),
		new model("res/skybox/skyboxSphere.obj"),
		2048.0f
	));
	waters::get()->setWater(new water(
		vector3(), vector3()
	));

	// Runs the framework loop.
	m_framework->run();

	// Deletes the framework.
	delete m_framework;

	// Pauses the console.
	std::cin.get();
	return 0;
}
