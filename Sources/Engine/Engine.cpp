#include "Engine.hpp"

#include "Maths/Maths.hpp"

namespace fl
{
	Engine *Engine::G_INSTANCE = nullptr;

	Engine::Engine() :
		m_start(HighResolutionClock::now()),
		m_timeOffset(0.0f),
		m_moduleRegister(nullptr),
		m_fpsLimit(-1.0f),
		m_initialized(false),
		m_running(true),
		m_error(false)
	{
		G_INSTANCE = this;

		m_moduleRegister = static_cast<ModuleRegister *>(malloc(sizeof(ModuleRegister)));
		new(m_moduleRegister) ModuleRegister();
	}

	Engine::~Engine()
	{
		delete m_moduleRegister;
	}

	int Engine::Run() const
	{
		try
		{
			while (m_running)
			{
				m_updater->Update(m_moduleRegister);
			}

			return EXIT_SUCCESS;
		}
		catch (const std::runtime_error &e)
		{
			fprintf(stderr, "Flounder has hit an exception!\n");
			fprintf(stderr, "%s\n", e.what());
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

	std::string Engine::GetDateTime()
	{
		time_t rawtime;
		struct tm *timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%Y-%m-%d-%I%M%S", timeinfo);
		std::string str = std::string(buffer);
		return str;
	}
}
