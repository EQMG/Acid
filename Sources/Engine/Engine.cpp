#include "Engine.hpp"

#include "Audio/Audio.hpp"
#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Devices/Window.hpp"
#include "Files/Files.hpp"
#include "Gizmos/Gizmos.hpp"
#include "Inputs/Input.hpp"
#include "Particles/Particles.hpp"
#include "Graphics/Graphics.hpp"
#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Timers/Timers.hpp"
#include "Uis/Uis.hpp"

#include "Config.hpp"

namespace acid {
Engine *Engine::Instance = nullptr;

Engine::Engine(std::string argv0, bool emptyRegister) :
	argv0(std::move(argv0)),
	version{ACID_VERSION_MAJOR, ACID_VERSION_MINOR, ACID_VERSION_PATCH},
	fpsLimit(-1.0f),
	running(true),
	elapsedUpdate(15.77ms),
	elapsedRender(-1s) {
	Instance = this;
	Log::OpenLog(Time::GetDateTime("Logs/%Y%m%d%H%M%S.txt"));

#if defined(ACID_DEBUG)
	Log::Out("Version: ", ACID_VERSION, '\n');
	Log::Out("Git: ", ACID_COMPILED_COMMIT_HASH, " on ", ACID_COMPILED_BRANCH, '\n');
	Log::Out("Compiled on: ", ACID_COMPILED_SYSTEM, " from: ", ACID_COMPILED_GENERATOR, " with: ", ACID_COMPILED_COMPILER, "\n\n");
#endif

	// Modules are not self registering so we can ensure regular initialization order.
	if (!emptyRegister) {
		Files::Register();
		Timers::Register();
		Resources::Register();
		Window::Register();
		Audio::Register();
		Joysticks::Register();
		Keyboard::Register();
		Mouse::Register();
		Graphics::Register();
		Input::Register();
		Scenes::Register();
		Gizmos::Register();
		Particles::Register();
		Shadows::Register();
		Uis::Register();
	}
}

Engine::~Engine() {
	Module::Registry().clear();
	Log::CloseLog();
}

int32_t Engine::Run() {
	while (running) {
		if (app) {
			if (!app->started) {
				app->Start();
				app->started = true;
			}
			
			app->Update();
		}

		elapsedRender.SetInterval(Time::Seconds(1.0f / fpsLimit));

		// Always-Update.
		UpdateStage(Module::Stage::Always);

		if (elapsedUpdate.GetElapsed() != 0) {
			// Resets the timer.
			ups.Update(Time::Now());

			// Pre-Update.
			UpdateStage(Module::Stage::Pre);

			// Update.
			UpdateStage(Module::Stage::Normal);

			// Post-Update.
			UpdateStage(Module::Stage::Post);

			// Updates the engines delta.
			deltaUpdate.Update();
		}

		// Prioritize updates over rendering.
		//if (!Maths::AlmostEqual(elapsedUpdate.GetInterval().AsSeconds(), deltaUpdate.change.AsSeconds(), 0.8f)) {
		//	continue;
		//}

		// Renders when needed.
		if (elapsedRender.GetElapsed() != 0) {
			// Resets the timer.
			fps.Update(Time::Now());

			// Render
			UpdateStage(Module::Stage::Render);

			// Updates the render delta, and render time extension.
			deltaRender.Update();
		}
	}

	return EXIT_SUCCESS;
}

void Engine::UpdateStage(Module::Stage stage) {
	for (auto &[stageIndex, module] : Module::Registry()) {
		if (stageIndex.first == stage)
			module->Update();
	}
}
}
