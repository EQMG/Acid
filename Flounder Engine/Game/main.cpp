#include <iostream>

#include "../Sources/framework/framework.h"
#include "../Sources/framework/glfw/glfwupdater.h"

#include "maincamera.h"

using namespace flounder;

int main() {
	framework *m_framework = new framework("Flounder C++", -1);
	m_framework->loadUpdater(new glfwupdater());
	camera::get()->loadCamera(new maincamera());
	m_framework->run();
	delete m_framework;
	std::cout << "Press enter to close the console!" << std::endl;
	std::getchar();
	return 0;
}