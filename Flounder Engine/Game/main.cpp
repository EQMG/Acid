#include <iostream>

#include "../Sources/framework/framework.h"
#include "../Sources/framework/glfw/glfwupdater.h"

#include "maincamera.h"
#include "mainplayer.h"
#include "maininstance.h"
#include "mainrenderer.h"

using namespace flounder;

int main() {
	// Creates the framework object.
	framework *m_framework = new framework("Flounder Game", -1);

	// Loads things to the framework.
	framework::get()->loadUpdater(new glfwupdater());
	display::get()->load(3, 2, 1080, 720, "Flounder Game", "Resources/flounder.png", false, true, 0, false);
	mouse::get()->load("Resources/guis/cursor.png");
	standards::get()->loadStandard(new maininstance());
	camera::get()->loadCamera(new maincamera());
	camera::get()->loadPlayer(new mainplayer());

	// Initializes the framework.
	m_framework->init();

	renderer::get()->loadRendererMaster(new mainrenderer());
	skybox::get()->loadTexture(texture::newTexture()
		->setCubemap(6, "Resources/skybox/starsRight.png", "Resources/skybox/starsLeft.png", "Resources/skybox/starsTop.png",
		"Resources/skybox/starsBottom.png", "Resources/skybox/starsBack.png", "Resources/skybox/starsFront.png")
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