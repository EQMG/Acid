#include <iostream>

#include "../Sources/framework/framework.h"
#include "../Sources/framework/glfw/glfwupdater.h"

#include "maincamera.h"
#include "mainplayer.h"

using namespace flounder;

int main() {
	framework *m_framework = new framework("Flounder C++", -1);
	m_framework->loadUpdater(new glfwupdater());

	camera::get()->loadCamera(new maincamera());
	camera::get()->loadPlayer(new mainplayer());
	
	auto meme = []() -> void 
	{
		std::cout << "Dank Memes!" << std::endl;
	};

	meme();

	// tasks::get()->addTask(NULL);

	m_framework->run();
	delete m_framework;

	std::cout << "Press enter to close the console!" << std::endl;
	std::getchar();
	return 0;
}