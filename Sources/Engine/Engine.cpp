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

namespace acid
{
using HighResolutionClock = std::chrono::high_resolution_clock;
using MicrosecondsType = std::chrono::duration<int64_t, std::micro>;

Engine *Engine::INSTANCE = nullptr;
std::chrono::time_point<HighResolutionClock> TIME_START = HighResolutionClock::now();

Engine::Engine(std::string argv0, const bool &emptyRegister) :
	m_argv0(std::move(argv0)),
	m_fpsLimit(-1.0f),
	m_running(true),
	m_timerUpdate(Time::Seconds(1.0f / 68.0f)),
	m_timerRender(Time::Seconds(1.0f / -1.0f)),
	m_ups(),
	m_fps()
{
	INSTANCE = this;
	Log::OpenLog("Logs/" + GetDateTime() + ".log");

	if (!emptyRegister)
	{
		AddModule<Files>(Module::Stage::Post);
		AddModule<Timers>(Module::Stage::Post);
		AddModule<Resources>(Module::Stage::Post);

		AddModule<Window>(Module::Stage::Always);
		AddModule<Audio>(Module::Stage::Always);
		AddModule<Joysticks>(Module::Stage::Always);
		AddModule<Keyboard>(Module::Stage::Always);
		AddModule<Mouse>(Module::Stage::Always);
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
		if (m_game != nullptr)
		{
			m_game->Update();
		}

		m_timerRender.SetInterval(Time::Seconds(1.0f / m_fpsLimit));

		// Always-Update.
		m_modules.UpdateStage(Module::Stage::Always);

		if (m_timerUpdate.IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate.ResetStartTime();
			m_ups.Update(GetTime().AsSeconds());

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
		if (!Maths::AlmostEqual(m_timerUpdate.GetInterval().AsSeconds(), m_deltaUpdate.GetChange().AsSeconds(), 0.8f))
		{
			continue;
		}

		// Renders when needed.
		if (m_timerRender.IsPassedTime())
		{
			// Resets the timer.
			m_timerRender.ResetStartTime();
			m_fps.Update(GetTime().AsSeconds());

			// Render
			m_modules.UpdateStage(Module::Stage::Render);

			// Updates the render delta, and render time extension.
			m_deltaRender.Update();
		}
	}

	return EXIT_SUCCESS;
}

Time Engine::GetTime()
{
	return Time::Microseconds(std::chrono::duration_cast<MicrosecondsType>(HighResolutionClock::now() - TIME_START).count());
}

std::string Engine::GetDateTime()
{
	time_t rawtime;
	char buffer[80];

	time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%Y-%m-%d-%I%M%S", timeinfo);
	return std::string(buffer);
}
}
