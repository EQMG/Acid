#include <iostream>
#include <Windows.h>

#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
	std::cout << "Hello World" << std::endl;
	std::cin.get();
	return 0;
}
