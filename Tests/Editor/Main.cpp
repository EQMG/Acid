#include <Engine/Engine.hpp>
#include <Files/Files.hpp>
#include <Graphics/Graphics.hpp>

#include "EditorRenderer.hpp"
#include "Plugins.hpp"

using namespace acid;
using namespace test;

int main(int argc, char **argv) {
	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);

	// Registers file search paths.
	Files::Get()->AddSearchPath("Resources/Engine");

	// Sets values to modules.
	Windows::Get()->GetWindow(0)->SetTitle("Acid Editor");
	Windows::Get()->GetWindow(0)->SetIcons({
		"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png",
		"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
	Graphics::Get()->SetRenderer(std::make_unique<EditorRenderer>());

	// Runs the game loop.
	auto exitCode = engine->Run();
	engine = nullptr;
	
	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}
