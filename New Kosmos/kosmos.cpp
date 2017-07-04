// kosmos.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "framework.h"
#include "updaterglfw.h"

namespace flounder {
	int main()
	{
		framework &framework = new framework(); // (std::string) "New Kosmos", new updaterglfw(), 60, new extension[0]
		framework->run();
		return 0;
	}
}