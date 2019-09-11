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

namespace acid
{
Engine *Engine::INSTANCE{nullptr};

Engine::Engine(std::string argv0, bool emptyRegister) :
	m_argv0{std::move(argv0)},
	m_version{ACID_VERSION_MAJOR, ACID_VERSION_MINOR, ACID_VERSION_PATCH},
	m_fpsLimit{-1.0f},
	m_running{true},
	m_elapsedUpdate{14.705ms},
	m_elapsedRender{-1s}
{
	INSTANCE = this;
	//Log::OpenLog(Time::GetDateTime("Logs/%Y%m%d%H%M%S.log"));

	std::cout << "Version: " << ACID_VERSION << '\n' <<
		"Git: " << ACID_COMPILED_COMMIT_HASH << " on " << ACID_COMPILED_BRANCH << '\n' <<
		"Compiled on: " << ACID_COMPILED_SYSTEM << " from: " << ACID_COMPILED_GENERATOR << " with: " << ACID_COMPILED_COMPILER << "\n\n";

	// TODO: Maybe move each module into a Singleton???
	if (!emptyRegister)
	{
		AddModule<Files>(Module::Stage::Post);
		AddModule<Timers>(Module::Stage::Post);
		AddModule<Resources>(Module::Stage::Post);

		AddModule<Window>(Module::Stage::Pre);
		AddModule<Audio>(Module::Stage::Pre);
		AddModule<Joysticks>(Module::Stage::Pre);
		AddModule<Keyboard>(Module::Stage::Pre);
		AddModule<Mouse>(Module::Stage::Pre);
		AddModule<Graphics>(Module::Stage::Render);
		
		AddModule<Scenes>(Module::Stage::Normal);
		AddModule<Gizmos>(Module::Stage::Normal);
		AddModule<Particles>(Module::Stage::Normal);
		AddModule<Shadows>(Module::Stage::Normal);
		AddModule<Uis>(Module::Stage::Normal);
	}
}

int32_t Engine::Run()
{
	while (m_running)
	{
		if (m_app)
		{
			m_app->Update();
		}

		m_elapsedRender.SetInterval(Time::Seconds(1.0f / m_fpsLimit));

		// Always-Update.
		m_modules.UpdateStage(Module::Stage::Always);

		if (m_elapsedUpdate.GetElapsed() != 0)
		{
			// Resets the timer.
			m_ups.Update(Time::Now());

			// Pre-Update.
			m_modules.UpdateStage(Module::Stage::Pre);

			// Update.
			m_modules.UpdateStage(Module::Stage::Normal);

			// Post-Update.
			m_modules.UpdateStage(Module::Stage::Post);

			// Updates the engines delta.
			m_deltaUpdate.Update();
		}

		// Prioritize updates over rendering.
		if (!Maths::AlmostEqual(m_elapsedUpdate.GetInterval().AsSeconds(), m_deltaUpdate.m_change.AsSeconds(), 0.8f))
		{
			continue;
		}

		// Renders when needed.
		if (m_elapsedRender.GetElapsed() != 0)
		{
			// Resets the timer.
			m_fps.Update(Time::Now());

			// Render
			m_modules.UpdateStage(Module::Stage::Render);

			// Updates the render delta, and render time extension.
			m_deltaRender.Update();
		}
	}

	return EXIT_SUCCESS;
}
}
