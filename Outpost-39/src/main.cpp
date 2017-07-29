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
	display::get()->setTitle("Outpost 39");
	display::get()->setIcon("res/icon.png");
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

	texture *textureIcon = texture::newTexture()->setFile("res/cursor.png")->create();

	mouse::get()->setCustomMouse("res/cursor.png");

	camera::get()->setCamera(new maincamera());
	camera::get()->setPlayer(new mainplayer());
	renderer::get()->setManager(new managerrender());
	uis::get()->setMaster(new manageruis());
	standards::get()->addStandard(new instance());

	// Runs the framework loop.
	m_framework->run();

	// Deletes the framework.
	delete textureIcon;
	delete m_framework;

	// Pauses the console.
	std::cout << "Press enter to close the console!" << std::endl;
	std::cin.get();
	return 0;
}
