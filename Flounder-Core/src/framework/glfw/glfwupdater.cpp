#include "glfwupdater.h"

namespace flounder
{
	glfwupdater::glfwupdater() :
		iupdater()
	{
	}

	glfwupdater::~glfwupdater()
	{
		for (std::multimap<float, std::pair<std::string, imodule*>>::iterator it = --m_modules->end(); it != m_modules->begin(); --it)
		{
			delete (*it).second.second;
		}

		delete m_modules;

		delete m_deltaRender;
		delete m_deltaUpdate;

		delete m_timerRender;
		delete m_timerUpdate;
	}

	void glfwupdater::create()
	{
		m_startTime = static_cast<float>(glfwGetTime());
		m_timeOffset = 0.0f;
		m_deltaUpdate = new delta();
		m_deltaRender = new delta();
		m_timerUpdate = new timer(1.0f / 62.0f);
		m_timerRender = new timer(1.0f / -1.0f);

		m_modules = new std::multimap<float, std::pair<std::string, imodule*>>();

		addModule(Render, "display", new display());
		addModule(UpdatePre, "audio", new audio());
		addModule(UpdatePre, "mouse", new mouse());
		addModule(UpdatePre, "keyboard", new keyboard());
		addModule(UpdatePre, "joysticks", new joysticks());
		addModule(UpdatePre, "events", new events());
		addModule(UpdatePre, "tasks", new tasks());
		addModule(UpdatePre, "processing", new processing());
		addModule(UpdatePre, "camera", new camera());
		addModule(Render, "renderer", new renderer());
		addModule(UpdatePre, "standards", new standards());
		addModule(UpdatePre, "uis", new uis());
		addModule(UpdatePre, "worlds", new worlds());
		addModule(Render, "shadows", new shadows());
		addModule(UpdatePre, "particles", new particles());
		addModule(UpdatePre, "skyboxes", new skyboxes());
		addModule(UpdatePre, "terrains", new terrains());
		addModule(UpdatePre, "waters", new waters());
	}

	void glfwupdater::update()
	{
		if (display::get() != nullptr)
		{
			m_timerRender->setInterval(1.0f / display::get()->getFpsLimit());
		}

		// Always-Update
		runUpdate(UpdateAlways);

		if (m_timerUpdate->isPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->resetStartTime();

			// Updates the frameworks delta.
			m_deltaUpdate->update();

			// Pre-Update
			runUpdate(UpdatePre);

			// Update
			runUpdate(Update);

			// Post-Update
			runUpdate(UpdatePost);
		}

		// Renders when needed.
		if (m_timerRender->isPassedTime() || display::get()->getFpsLimit() <= 0.0f || display::get()->getFpsLimit() > 1000.0f)
		{
			// if (maths::almostEqual(m_timerUpdate->getInterval(), m_deltaUpdate->getChange(), 7.0f)) {}

			// Resets the timer.
			m_timerRender->resetStartTime();

			// Updates the render delta, and render time extension.
			m_deltaRender->update();

			// Render
			runUpdate(Render);
		}
	}

	void glfwupdater::addModule(moduleupdate typeUpdate, std::string moduleName, imodule *object)
	{
		float offset = typeUpdate + (0.01f * static_cast<float>(m_modules->size()));
		m_modules->insert(std::make_pair(offset, std::make_pair(moduleName, object)));
	}

	imodule *flounder::glfwupdater::getInstance(const std::string &name)
	{
		for (std::multimap<float, std::pair<std::string, imodule*>>::iterator it = m_modules->begin(); it != m_modules->end(); ++it)
		{
			if ((*it).second.first == name)
			{
				return (*it).second.second;
			}
		}

		return nullptr;
	}

	void glfwupdater::runUpdate(moduleupdate typeUpdate)
	{
		for (std::multimap<float, std::pair<std::string, imodule*>>::iterator it = m_modules->begin(); it != m_modules->end(); ++it)
		{
			if (static_cast<int>(floor((*it).first)) == typeUpdate)
			{
				(*it).second.second->update();
			}
		}
	}
}
