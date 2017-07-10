#include <iostream>

#include "../Sources/framework/framework.h"
#include "../Sources/framework/glfw/glfwupdater.h"

#include "maincamera.h"
#include "mainplayer.h"

#include "../Sources/inputs/buttonmouse.h"
#include "../Sources/events/ieventstandard.h"

using namespace flounder;

int main() {
	framework *m_framework = new framework("Flounder Game", -1);
	m_framework->loadUpdater(new glfwupdater());

	display::get()->load(3, 2, 1080, 720, "Flounder Game", new file("res/flounder.png"), false, true, 0, false);

	camera::get()->loadCamera(new maincamera());
	camera::get()->loadPlayer(new mainplayer());
	
	buttonmouse *m_buttonLeft = new buttonmouse(1, GLFW_MOUSE_BUTTON_LEFT);
	events::get()->addEvent(
		new ieventstandard(false, [&]() { 
			return m_buttonLeft->wasDown();
		}, []() {
			logger::get()->log("Left mouse button was down!"); 
		})
	);

	tasks::get()->addTask([&]() {
		logger::get()->log("Hello world!");
	});

	m_framework->run();
	delete m_buttonLeft;
	delete m_framework;

	std::cout << "Press enter to close the console!" << std::endl;
	std::cin.get();
	return 0;
}