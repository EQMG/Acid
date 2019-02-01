#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <Engine/Engine.hpp>
#include <Files/FileSystem.hpp>
#include <Files/FileWatcher.hpp>

#define CR_HOST CR_UNSAFE
#include "cr.h"

using namespace acid;

int main(int argc, char **argv)
{
	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);

	// Loads a game from a DLL.
	std::string loadedPath = R"(C:\Users\mattp\CMakeBuilds\b79ccb6f-d6b4-5e3c-8bf8-1f7f40a18556\build\x64-Debug (default)\bin\TestGUI.dll)"; // C:\Users\mattp\Documents\Acid Workspace\Acid\cmake-build-debug\bin\TestGUI.dll
	cr_plugin ctx;
	cr_plugin_load(ctx, loadedPath.c_str());
	cr_plugin_update(ctx);
	fflush(stdout);
	fflush(stderr);

	// Watches the DLL path.
	FileWatcher watcher = FileWatcher(FileSystem::GetWorkingDirectory(), Time::Seconds(2.0f));
	watcher.GetOnChange() += [&](std::string path, FileWatcher::Status status)
	{
		if (path == loadedPath)
		{
			cr_plugin_update(ctx);
			fflush(stdout);
			fflush(stderr);
		}
	};

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	cr_plugin_close(ctx);
	return EXIT_SUCCESS;
}