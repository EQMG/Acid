#include <iostream>

#include "framework\framework.h"
#include "framework\glfw\glfwupdater.h"

using namespace flounder;

int main() {
	framework("Flounder C++", -1).run(new glfwupdater());
	std::cout << "Press enter to close the console!" << std::endl;
	std::getchar();
	return 0;
}