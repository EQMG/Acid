#include "Engine.hpp"

#include <chrono>

namespace acid
{
using HighResolutionClock = std::chrono::high_resolution_clock;
using MicrosecondsType = std::chrono::duration<int64_t, std::micro>;

Engine *Engine::INSTANCE = nullptr;
std::chrono::time_point<HighResolutionClock> TIME_START = HighResolutionClock::now();

Engine::Engine(std::string argv0, const bool &emptyRegister) :
	m_game(nullptr),
	m_argv0(std::move(argv0)),
	m_fpsLimit(-1.0f),
	m_running(true),
	m_error(false)
{
	INSTANCE = this;
	Log::OpenLog("Logs/" + GetDateTime() + ".log");

	if (!emptyRegister)
	{
		m_moduleManager.FillRegister();
	}
}

int32_t Engine::Run()
{
	while (m_running)
	{
		if (m_game != nullptr)
		{
			if (!m_game->m_started)
			{
				m_game->Start();
				m_game->m_started = true;
			}

			m_game->Update();
		}

		m_moduleUpdater.Update(m_moduleManager);
	}

	return EXIT_SUCCESS;
}

void Engine::RequestClose(const bool &error)
{
	m_running = false;

	// A statement in case it was already true.
	if (error)
	{
		m_error = true;
	}
}

Time Engine::GetTime()
{
	auto duration = Time::Microseconds(std::chrono::duration_cast<MicrosecondsType>(HighResolutionClock::now() - TIME_START).count());

	if (INSTANCE == nullptr)
	{
		return duration;
	}

	return duration + INSTANCE->m_timeOffset;
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
