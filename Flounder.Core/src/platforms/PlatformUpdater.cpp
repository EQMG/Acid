#include "PlatformUpdater.hpp"

#include "../camera/camera.hpp"
#include "../devices/Audio.hpp"
#include "../devices/Display.hpp"
#include "../devices/Joysticks.hpp"
#include "../devices/Keyboard.hpp"
#include "../devices/Mouse.hpp"
#include "../events/Events.hpp"
#include "../particles/Particles.hpp"
#include "../processing/Processing.hpp"
#include "../renderer/Renderer.hpp"
#include "../shadows/Shadows.hpp"
#include "../skyboxes/Skyboxes.hpp"
#include "../standards/Standards.hpp"
#include "../tasks/Tasks.hpp"
#include "../terrains/Terrains.hpp"
#include "../uis/Uis.hpp"
#include "../waters/Waters.hpp"
#include "../worlds/Worlds.hpp"

namespace Flounder
{
	PlatformUpdater::PlatformUpdater() :
		IUpdater(),
		m_startTime(0.0f),
		m_timeOffset(0.0f),
		m_deltaUpdate(nullptr),
		m_deltaRender(nullptr),
		m_timerUpdate(nullptr),
		m_timerRender(nullptr),
		m_timerLog(nullptr),
		m_modules(new std::multimap<float, std::pair<std::string, IModule*>>())
	{
	}

	PlatformUpdater::~PlatformUpdater()
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
		delete m_timerLog;
	}

	void PlatformUpdater::Create()
	{
		m_startTime = static_cast<float>(glfwGetTime());
		m_timeOffset = 0.0f;
		m_deltaUpdate = new Delta();
		m_deltaRender = new Delta();
		m_timerUpdate = new Timer(1.0f / 62.0f);
		m_timerRender = new Timer(1.0f / -1.0f);
		m_timerLog = new Timer(2.0f);

		AddModule(UpdatePre, "audio", new Audio());
		AddModule(UpdateRender, "display", new Display());
		AddModule(UpdatePre, "joysticks", new Joysticks());
		AddModule(UpdatePre, "keyboard", new Keyboard());
		AddModule(UpdatePre, "mouse", new Mouse());
		AddModule(UpdateRender, "renderer", new Renderer());

		AddModule(UpdatePre, "events", new Events());
		AddModule(UpdatePre, "tasks", new Tasks());
		AddModule(UpdatePre, "processing", new Processing());
		AddModule(UpdatePre, "standards", new Standards());
		AddModule(UpdatePre, "camera", new Camera());
		AddModule(UpdatePre, "uis", new Uis());
		AddModule(UpdatePre, "worlds", new Worlds());
		AddModule(UpdateRender, "shadows", new Shadows());
		AddModule(UpdatePre, "particles", new Particles());
		AddModule(UpdatePre, "skyboxes", new Skyboxes());
		AddModule(UpdatePre, "terrains", new Terrains());
		AddModule(UpdatePre, "waters", new Waters());
	}

	void PlatformUpdater::Update()
	{
		if (Display::Get() != nullptr)
		{
			m_timerRender->SetInterval(1.0f / Display::Get()->GetFpsLimit());
		}

		// Always-Update
		RunUpdate(UpdateAlways);

		if (m_timerUpdate->IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->ResetStartTime();

			// Updates the engines delta.
			m_deltaUpdate->Update();

			// Pre-Update
			RunUpdate(UpdatePre);

			// Update
			RunUpdate(UpdateNormal);

			// Post-Update
			RunUpdate(UpdatePost);
		}

		// Renders when needed.
		if (m_timerRender->IsPassedTime() || Display::Get()->GetFpsLimit() <= 0.0f || Display::Get()->GetFpsLimit() > 1000.0f)
		{
			// if (maths::almostEqual(m_timerUpdate->getInterval(), m_deltaUpdate->getChange(), 7.0f)) {}

			// Resets the timer.
			m_timerRender->ResetStartTime();

			// Updates the render delta, and render time extension.
			m_deltaRender->Update();

			// Render
			RunUpdate(UpdateRender);
		}

		// Logs when needed.
		if (m_timerLog->IsPassedTime())
		{
			// Resets the timer.
			m_timerLog->ResetStartTime();

			printf("FPS: %u\n", static_cast<int>(1.0f / m_deltaRender->GetChange()));
		}
	}

	void PlatformUpdater::AddModule(ModuleUpdate typeUpdate, std::string moduleName, IModule *module)
	{
		float offset = typeUpdate + (0.01f * static_cast<float>(m_modules->size()));
		m_modules->insert(make_pair(offset, make_pair(moduleName, module)));
	}

	IModule *PlatformUpdater::GetModule(const std::string &name)
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

	void PlatformUpdater::RunUpdate(ModuleUpdate typeUpdate) const
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
