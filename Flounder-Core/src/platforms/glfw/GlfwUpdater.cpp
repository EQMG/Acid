#include "glfwupdater.hpp"

#include "../../camera/camera.hpp"
#include "../../devices/audio.hpp"
#include "../../devices/display.hpp"
#include "../../devices/joysticks.hpp"
#include "../../devices/keyboard.hpp"
#include "../../devices/mouse.hpp"
#include "../../renderer/renderer.hpp"

#include "../../events/events.hpp"
#include "../../processing/processing.hpp"
#include "../../shadows/shadows.hpp"
#include "../../standards/standards.hpp"
#include "../../tasks/tasks.hpp"
#include "../../terrains/terrains.hpp"
#include "../../uis/uis.hpp"
#include "../../particles/particles.hpp"
#include "../../skyboxes/skyboxes.hpp"
#include "../../waters/waters.hpp"
#include "../../worlds/worlds.hpp"

namespace Flounder
{
	GlfwUpdater::GlfwUpdater() :
		IUpdater(),
		m_startTime(0.0f),
		m_timeOffset(0.0f),
		m_deltaUpdate(nullptr),
		m_deltaRender(nullptr),
		m_timerUpdate(nullptr),
		m_timerRender(nullptr),
		m_modules(new std::multimap<float, std::pair<std::string, IModule*>>())
	{
	}

	GlfwUpdater::~GlfwUpdater()
	{
		for (auto it = --m_modules->end(); it != m_modules->begin(); --it)
		{
			delete (*it).second.second;
		}

		delete m_modules;

		delete m_deltaRender;
		delete m_deltaUpdate;

		delete m_timerRender;
		delete m_timerUpdate;
	}

	void GlfwUpdater::Create()
	{
		m_startTime = static_cast<float>(glfwGetTime());
		m_timeOffset = 0.0f;
		m_deltaUpdate = new delta();
		m_deltaRender = new delta();
		m_timerUpdate = new timer(1.0f / 62.0f);
		m_timerRender = new timer(1.0f / -1.0f);

		AddModule(UpdatePre, "audio", new audio());
		AddModule(UpdateRender, "display", new display());
		AddModule(UpdatePre, "joysticks", new joysticks());
		AddModule(UpdatePre, "keyboard", new keyboard());
		AddModule(UpdatePre, "mouse", new mouse());
		AddModule(UpdateRender, "renderer", new renderer());

		AddModule(UpdatePre, "events", new events());
		AddModule(UpdatePre, "tasks", new tasks());
		AddModule(UpdatePre, "processing", new processing());
		AddModule(UpdatePre, "standards", new standards());
		AddModule(UpdatePre, "camera", new Camera());
		AddModule(UpdatePre, "uis", new uis());
		AddModule(UpdatePre, "worlds", new worlds());
		AddModule(UpdateRender, "shadows", new shadows());
		AddModule(UpdatePre, "particles", new particles());
		AddModule(UpdatePre, "skyboxes", new skyboxes());
		AddModule(UpdatePre, "terrains", new terrains());
		AddModule(UpdatePre, "waters", new waters());
	}

	void GlfwUpdater::Update()
	{
		if (display::get() != nullptr)
		{
			m_timerRender->setInterval(1.0f / display::get()->getFpsLimit());
		}

		// Always-Update
		RunUpdate(UpdateAlways);

		if (m_timerUpdate->isPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->resetStartTime();

			// Updates the engines delta.
			m_deltaUpdate->update();

			// Pre-Update
			RunUpdate(UpdatePre);

			// Update
			RunUpdate(UpdateNormal);

			// Post-Update
			RunUpdate(UpdatePost);
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
			RunUpdate(UpdateRender);
		}
	}

	void GlfwUpdater::AddModule(ModuleUpdate typeUpdate, std::string moduleName, IModule *module)
	{
		float offset = typeUpdate + (0.01f * static_cast<float>(m_modules->size()));
		m_modules->insert(std::make_pair(offset, std::make_pair(moduleName, module)));
	}

	IModule *GlfwUpdater::GetModule(const std::string &name)
	{
		for (auto it = m_modules->begin(); it != m_modules->end(); ++it)
		{
			if ((*it).second.first == name)
			{
				return (*it).second.second;
			}
		}

		return nullptr;
	}

	void GlfwUpdater::RunUpdate(ModuleUpdate typeUpdate)
	{
		for (auto it = m_modules->begin(); it != m_modules->end(); ++it)
		{
			if (static_cast<int>(floor((*it).first)) == typeUpdate)
			{
				(*it).second.second->Update();
			}
		}
	}
}
