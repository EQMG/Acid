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

	void framework::run(updater* updater)
	{
		m_updater = updater;
		m_updater->init();

		while (m_running) 
		{
			m_updater->update();
		}
	}

	module *framework::getInstance(std::string name)
	{
		return m_updater->getInstance(name);
	}

	std::string framework::getUnlocalizedName()
	{
		return m_unlocalizedName;
	}

	float framework::getTimeOffset()
	{
		return 0.0f; //  updater::getTimeOffset();
	}

	void framework::setTimeOffset(float timeOffset)
	{
		// updater::setTimeOffset(timeOffset);
	}

	float framework::getDelta()
	{
		return 0.0f; // updater::getDelta();
	}

	float framework::getDeltaRender()
	{
		return 0.0f; // updater::getDeltaRender();
	}

	float framework::getTimeSec()
	{
		return 0.0f; // updater::getTimeSec();
	}

	float framework::getTimeMs()
	{
		return 0.0f; // updater::getTimeMs();
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
	}
}