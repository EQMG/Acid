#include "framework.hpp"

namespace flounder
{
	framework *framework::G_INSTANCE = nullptr;

	framework::framework() :
		m_initialized(false),
		m_running(true),
		m_error(false),
		m_updater(nullptr)
	{
		G_INSTANCE = this;
	}

	framework::~framework()
	{
		delete m_updater;
	}

	void framework::setUpdater(iupdater *updater)
	{
		m_updater = updater;
		m_updater->create();
	}

	void framework::run()
	{
		while (m_running)
		{
			m_updater->update();
		}
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
