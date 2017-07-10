#include <iostream>

#include "../Sources/framework/framework.h"
#include "../Sources/framework/glfw/glfwupdater.h"

#include "maincamera.h"
#include "mainplayer.h"

#include "../Sources/inputs/buttonmouse.h"
#include "../Sources/events/ieventstandard.h"

using namespace flounder;

int main() {
	framework *m_framework = new framework("Flounder C++", -1);
	m_framework->loadUpdater(new glfwupdater());

	camera::get()->loadCamera(new maincamera());
	camera::get()->loadPlayer(new mainplayer());
	
	buttonmouse *buttonLeft = new buttonmouse(1, GLFW_MOUSE_BUTTON_LEFT);
	events::get()->addEvent(
		new ieventstandard(false, [&]() { 
			return buttonLeft->wasDown(); 
		}, []() {
			logger::get()->log("Left mouse button was down!"); 
		})
	);

	tasks::get()->addTask([&]() {
		logger::get()->log("Hello world!");
	});

	m_framework->run();
	delete buttonLeft;
	delete m_framework;

	std::cout << "Press enter to close the console!" << std::endl;
	std::cin.get();
	return 0;
}