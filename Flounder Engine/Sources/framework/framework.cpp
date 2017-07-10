#include "framework.h"

namespace flounder {
	framework *framework::G_INSTANCE = NULL;

	framework::framework(const std::string & unlocalizedName, int fpsLimit)
	{
		// Sets the static object to this new one.
		framework::G_INSTANCE = this;

		// Sets the instances name.
		m_unlocalizedName = unlocalizedName;

		m_updater = NULL;
		
		m_initialized = false;
		m_running = true;
		m_error = false;
		m_fpsLimit = fpsLimit;
	}

	framework::~framework()
	{
		delete m_updater;
	}

	void framework::loadUpdater(iupdater *iupdater)
	{
		m_updater = iupdater;
		m_updater->create();
	}

	void framework::run()
	{
		m_updater->init();

		while (m_running)
		{
			m_updater->update();
		}
	}

	imodule *framework::getInstance(std::string name)
	{
		return m_updater->getInstance(name);
	}

	std::string framework::getUnlocalizedName()
	{
		return m_unlocalizedName;
	}

	bool framework::isInitialized()
	{
		return m_initialized;
	}

	void framework::setInitialized(bool initialized)
	{
		m_initialized = initialized;
	}

	bool framework::isRunning()
	{
		return m_running;
	}

	void framework::requestClose(bool error)
	{
		m_running = false;

		// A statement in case it was already true.
		if (error)
		{
			m_error = true;
		}
	}

	int framework::getFpsLimit()
	{
		return m_fpsLimit;
	}

	void framework::setFpsLimit(int fpsLimit)
	{
		m_fpsLimit = fpsLimit;
		m_updater->setFpsLimit(fpsLimit);
	}
}