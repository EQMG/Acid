#include "Engine.hpp"

#include "Maths/Maths.hpp"

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
	m_argv0(std::move(argv0)),
	m_version{ACID_VERSION_MAJOR, ACID_VERSION_MINOR, ACID_VERSION_PATCH},
	m_fpsLimit(-1.0f),
	m_running(true),
	m_elapsedUpdate(15.77ms),
	m_elapsedRender(-1s) {
	Instance = this;
	Log::OpenLog(Time::GetDateTime("Logs/%Y%m%d%H%M%S.txt"));

#if defined(ACID_DEBUG)
	Log::Out("Version: ", ACID_VERSION, '\n');
	Log::Out("Git: ", ACID_COMPILED_COMMIT_HASH, " on ", ACID_COMPILED_BRANCH, '\n');
	Log::Out("Compiled on: ", ACID_COMPILED_SYSTEM, " from: ", ACID_COMPILED_GENERATOR, " with: ", ACID_COMPILED_COMPILER, "\n\n");
#endif

	if (!emptyRegister) {
		Files::Register(Module::Stage::Post);
		Timers::Register(Module::Stage::Post);
		Resources::Register(Module::Stage::Post);

		Window::Register(Module::Stage::Pre);
		Audio::Register(Module::Stage::Pre);
		Joysticks::Register(Module::Stage::Pre);
		Keyboard::Register(Module::Stage::Pre);
		Mouse::Register(Module::Stage::Pre);
		Graphics::Register(Module::Stage::Render);

		Input::Register(Module::Stage::Normal);
		Scenes::Register(Module::Stage::Normal);
		Gizmos::Register(Module::Stage::Normal);
		Particles::Register(Module::Stage::Normal);
		Shadows::Register(Module::Stage::Normal);
		Uis::Register(Module::Stage::Normal);
	}
}

Engine::~Engine() {
	Module::Registry().clear();
	Log::CloseLog();
}

int32_t Engine::Run() {
	while (m_running) {
		if (m_app) {
			if (!m_app->m_started) {
				m_app->Start();
				m_app->m_started = true;
			}
			
			m_app->Update();
		}

		m_elapsedRender.SetInterval(Time::Seconds(1.0f / m_fpsLimit));

		// Always-Update.
		UpdateStage(Module::Stage::Always);

		if (m_elapsedUpdate.GetElapsed() != 0) {
			// Resets the timer.
			m_ups.Update(Time::Now());

			// Pre-Update.
			UpdateStage(Module::Stage::Pre);

			// Update.
			UpdateStage(Module::Stage::Normal);

			// Post-Update.
			UpdateStage(Module::Stage::Post);

			// Updates the engines delta.
			m_deltaUpdate.Update();
		}

		// Prioritize updates over rendering.
		//if (!Maths::AlmostEqual(m_elapsedUpdate.GetInterval().AsSeconds(), m_deltaUpdate.m_change.AsSeconds(), 0.8f)) {
		//	continue;
		//}

		// Renders when needed.
		if (m_elapsedRender.GetElapsed() != 0) {
			// Resets the timer.
			m_fps.Update(Time::Now());

			// Render
			UpdateStage(Module::Stage::Render);

			// Updates the render delta, and render time extension.
			m_deltaRender.Update();
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
