#include <iostream>

#include "../Sources/framework/framework.h"
#include "../Sources/framework/glfw/glfwupdater.h"

#include "maincamera.h"
#include "mainplayer.h"
#include "maininstance.h"
#include "mainrenderer.h"

#include "../Sources/inputs/buttonmouse.h"
#include "../Sources/events/ieventstandard.h"
#include "../Sources/helpers/helperfile.h"

using namespace flounder;

int main() {
	// Creates the framework object.
	framework *m_framework = new framework("Flounder Game", -1);

	// Loads things to the framework.
	framework::get()->loadUpdater(new glfwupdater());
	display::get()->load(3, 2, 1080, 720, "Flounder Game", "Resources/flounder.png", false, true, 0, false);
	standards::get()->loadStandard(new maininstance());
	camera::get()->loadCamera(new maincamera());
	camera::get()->loadPlayer(new mainplayer());

	// Initalizes the framework.
	m_framework->init();

	renderer::get()->loadRendererMaster(new mainrenderer());
	skybox::get()->loadTexture(
		new texture(6, "Resources/skybox/starsRight.png", "Resources/skybox/starsLeft.png", "Resources/skybox/starsTop.png",
			"Resources/skybox/starsBottom.png", "Resources/skybox/starsBack.png", "Resources/skybox/starsFront.png")
	);
	
	// Adds game elements to the framework.
	buttonmouse *m_buttonLeft = new buttonmouse(1, GLFW_MOUSE_BUTTON_LEFT);
	events::get()->addEvent(
		new ieventstandard(false, [&]() { 
			return m_buttonLeft->wasDown();
		}, []() {
			logger::get()->log("Left mouse button was down!"); 
		})
	);

	// Runs the framework loop.
	m_framework->run();

	// Deletes the framework.
	delete m_buttonLeft;
	delete m_framework;

	// Pauses the console.
	std::cout << "Press enter to close the console!" << std::endl;
	std::cin.get();
	return 0;
}