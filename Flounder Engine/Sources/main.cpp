#include <iostream>

#include "logger\logger.h"
#include "devices\display.h"
#include "devices\joysticks.h"
#include "devices\mouse.h"
#include "devices\keyboard.h"

#include "framework\framework.h"

using namespace flounder;

int main() {
	framework *l_framework = new framework("Flounder C++", -1);
	l_framework->run();
	delete l_framework;
	system("PAUSE");
	return 0;
}