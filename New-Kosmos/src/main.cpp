#include <iostream>

#include <flounder.h>

#include "maincamera.h"
#include "mainplayer.h"
#include "manageruis.h"
#include "instance.h"
#include "managerrender.h"

using namespace flounder;

int main()
{
	// Creates the framework object.
	framework *m_framework = new framework();
	m_framework->setUpdater(new glfwupdater());

	// Initializes the framework modules.
	display::get()->setWindowSize(1080, 720);
	display::get()->setTitle("New Kosmos");
	display::get()->setIcon("res/newkosmos.png");
#ifndef FLOUNDER_PLATFORM_WEB
	display::get()->setFpsLimit(0);
	display::get()->setVSync(false);
#else
	display::get()->setFpsLimit(60);
	//	display::get()->setVSync(true);
#endif
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
		texture::newTexture()
			->setCubemap(6, "res/skybox/starsRight.png", "res/skybox/starsLeft.png", "res/skybox/starsTop.png", "res/skybox/starsBottom.png", "res/skybox/starsBack.png", "res/skybox/starsFront.png")
			->create(),
		model::newModel()->setFile("res/skybox/skyboxSphere.obj")->create(),
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
