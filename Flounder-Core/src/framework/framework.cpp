#include "framework.h"

namespace flounder
{
	framework *framework::G_INSTANCE = NULL;

	framework::framework()
	{
		// Sets the static object to this new one.
		framework::G_INSTANCE = this;

		m_updater = NULL;

		m_initialized = false;
		m_running = true;
		m_error = false;
	}

	framework::~framework()
	{
		delete m_updater;
	}

	void framework::load(iupdater *updater)
	{
		m_updater = updater;
		m_updater->create();
	}

	void framework::init()
	{
		m_updater->init();
	}

	void framework::run()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		std::function<void()> mainLoop = [&]() {
			while (m_running)
			{
				m_updater->update();
			}
		};
		emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
		while (m_running)
		{
			m_updater->update();
		}
#endif
	}

	imodule *framework::getInstance(const std::string &name)
	{
		return m_updater->getInstance(name);
	}

	void framework::requestClose(const bool &error)
	{
		m_running = false;

		// A statement in case it was already true.
		if (error)
		{
			m_error = true;
		}
	}
}
