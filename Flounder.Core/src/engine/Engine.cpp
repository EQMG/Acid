#include "Engine.hpp"

namespace Flounder
{
	Engine *Engine::g_instance = nullptr;

	Engine::Engine() :
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

	void Engine::Run() const
	{
		while (m_running)
		{
			m_updater->Update();
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
