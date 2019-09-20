#include "Engine.hpp"

#include "Maths/Maths.hpp"

#include "Audio/Audio.hpp"
#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Devices/Window.hpp"
#include "Files/Files.hpp"
#include "Gizmos/Gizmos.hpp"
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
	m_elapsedUpdate(14.705ms),
	m_elapsedRender(-1s) {
	Instance = this;
	Log::OpenLog(Time::GetDateTime("Logs/%Y%m%d%H%M%S.txt"));

	Log::Out("Version: ", ACID_VERSION, '\n');
	Log::Out("Git: ", ACID_COMPILED_COMMIT_HASH, " on ", ACID_COMPILED_BRANCH, '\n');
	Log::Out("Compiled on: ", ACID_COMPILED_SYSTEM, " from: ", ACID_COMPILED_GENERATOR, " with: ", ACID_COMPILED_COMPILER, "\n\n");

	if (!emptyRegister) {
		Files::Register(ModuleStage::Post);
		Timers::Register(ModuleStage::Post);
		Resources::Register(ModuleStage::Post);

		Window::Register(ModuleStage::Pre);
		Audio::Register(ModuleStage::Pre);
		Joysticks::Register(ModuleStage::Pre);
		Keyboard::Register(ModuleStage::Pre);
		Mouse::Register(ModuleStage::Pre);
		Graphics::Register(ModuleStage::Render);

		Scenes::Register(ModuleStage::Normal);
		Gizmos::Register(ModuleStage::Normal);
		Particles::Register(ModuleStage::Normal);
		Shadows::Register(ModuleStage::Normal);
		Uis::Register(ModuleStage::Normal);
	}
}

Engine::~Engine() {
	//Module::Registry().clear();
	Log::CloseLog();
}

int32_t Engine::Run() {
	while (m_running) {
		if (m_app) {
			m_app->Update();
		}

		m_elapsedRender.SetInterval(Time::Seconds(1.0f / m_fpsLimit));

		// Always-Update.
		UpdateStage(ModuleStage::Always);

		if (m_elapsedUpdate.GetElapsed() != 0) {
			// Resets the timer.
			m_ups.Update(Time::Now());

			// Pre-Update.
			UpdateStage(ModuleStage::Pre);

			// Update.
			UpdateStage(ModuleStage::Normal);

			// Post-Update.
			UpdateStage(ModuleStage::Post);

			// Updates the engines delta.
			m_deltaUpdate.Update();
		}

		// Prioritize updates over rendering.
		if (!Maths::AlmostEqual(m_elapsedUpdate.GetInterval().AsSeconds(), m_deltaUpdate.m_change.AsSeconds(), 0.8f)) {
			continue;
		}

		// Renders when needed.
		if (m_elapsedRender.GetElapsed() != 0) {
			// Resets the timer.
			m_fps.Update(Time::Now());

			// Render
			UpdateStage(ModuleStage::Render);

			// Updates the render delta, and render time extension.
			m_deltaRender.Update();
		}
	}

	return EXIT_SUCCESS;
}

void Engine::UpdateStage(ModuleStage stage) {
	for (auto &[stageIndex, module] : Module::Registry()) {
		if (stageIndex.first == stage)
			module->Update();
	}
}
}
