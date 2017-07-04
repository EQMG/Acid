// kosmos.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "framework.h"

int main()
{
	framework framework = new framework((std::string) "New Kosmos", NULL, 60, new extension[0]);
	framework.run();
    return 0;
}
