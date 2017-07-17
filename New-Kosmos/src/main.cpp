#include <iostream>

#include "flounder.h"
#include "framework/framework.h"
#include "framework/glfw/glfwupdater.h"

#include "maincamera.h"
#include "mainplayer.h"
#include "maininstance.h"
#include "mainrenderer.h"

using namespace flounder;

int main() {
	// Creates the framework object.
	framework *m_framework = new framework(-1);

	// Loads things to the framework.
	framework::get()->loadUpdater(new glfwupdater());

	display::get()->load(3, 2, 1080, 720, "New Kosmos", "res/newkosmos.png", false, true, 0, false);
	mouse::get()->load("res/cursor.png");

	standards::get()->loadStandard(new maininstance());
	camera::get()->loadCamera(new maincamera());
	camera::get()->loadPlayer(new mainplayer());

	// Initializes the framework.
	m_framework->init();

	renderer::get()->loadRendererMaster(new mainrenderer());
	skybox::get()->loadTexture(texture::newTexture()
		->setCubemap(6, "res/skybox/starsRight.png", "res/skybox/starsLeft.png", "res/skybox/starsTop.png",
		"res/skybox/starsBottom.png", "res/skybox/starsBack.png", "res/skybox/starsFront.png")
		->create()
	);

	// Runs the framework loop.
	m_framework->run();

	// Deletes the framework.
	delete m_framework;

	// Pauses the console.
	std::cout << "Press enter to close the console!" << std::endl;
	std::cin.get();
	return 0;
}