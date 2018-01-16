#include "Engine.hpp"

#include <stdexcept>

namespace Flounder
{
	Engine *Engine::g_instance = nullptr;

	Engine::Engine() :
		m_start(HighResolutionClock::now()),
		m_timeOffset(0.0f),
		m_initialized(false),
		m_running(true),
		m_error(false),
		m_updater(nullptr)
	{
		g_instance = this;
	}

	Engine::~Engine()
	{
		delete m_updater;
	}

	void Engine::SetUpdater(IUpdater *updater)
	{
		m_updater = updater;
		m_updater->Create();
	}

	int Engine::Run() const
	{
		 try
		 {
			while (m_running)
			{
				m_updater->Update();
			}

			return EXIT_SUCCESS;
		 }
		 catch (const std::runtime_error &e)
		 {
		 	printf("%s\n", e.what());
		 	return EXIT_FAILURE;
		 }
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
}
