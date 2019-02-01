#include <iostream>
#include <Engine/Engine.hpp>
#include "Plugins.hpp"

using namespace acid;
using namespace test;

int main(int argc, char **argv)
{
	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);

	// Registers modules.
	auto &moduleManager = Engine::Get()->GetModuleManager();
	moduleManager.Add<Plugins>(Module::Stage::Always);

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}