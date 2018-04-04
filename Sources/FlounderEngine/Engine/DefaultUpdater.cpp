#include <Objects/ComponentRegister.hpp>
#include "DefaultUpdater.hpp"

#include "../Devices/Audio.hpp"
#include "../Devices/Joysticks.hpp"
#include "../Devices/Keyboard.hpp"
#include "../Devices/Mouse.hpp"
#include "../Scenes/Scenes.hpp"
#include "../Events/Events.hpp"
#include "../Particles/Particles.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Shadows/Shadows.hpp"
#include "../Terrains/Terrains.hpp"
#include "../Uis/Uis.hpp"
#include "../Waters/Waters.hpp"
#include "../Worlds/Worlds.hpp"

namespace Flounder
{
	DefaultUpdater::DefaultUpdater() :
		IUpdater(),
		m_deltaUpdate(nullptr),
		m_deltaRender(nullptr),
		m_timerUpdate(nullptr),
		m_timerRender(nullptr),
		m_modules(new std::multimap<float, std::pair<std::string, IModule *>>())
	{
	}

	DefaultUpdater::~DefaultUpdater()
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

	void DefaultUpdater::Create()
	{
		m_deltaUpdate = new Delta();
		m_deltaRender = new Delta();
		m_timerUpdate = new Timer(1.0f / 66.0f);
		m_timerRender = new Timer(1.0f / -1.0f);

		ModuleCreate<Audio>(UpdatePre, "audio");
		ModuleCreate<Display>(UpdatePost, "display");
		ModuleCreate<Joysticks>(UpdatePre, "joysticks");
		ModuleCreate<Keyboard>(UpdatePre, "keyboard");
		ModuleCreate<Mouse>(UpdatePre, "mouse");
		ModuleCreate<Scenes>(UpdateNormal, "scenes");
		ModuleCreate<Renderer>(UpdateRender, "renderer");
		ModuleCreate<Resources>(UpdatePre, "resources");
		ModuleCreate<Events>(UpdatePre, "events");
		ModuleCreate<Tasks>(UpdatePre, "tasks");
		ModuleCreate<Uis>(UpdatePre, "uis");
		ModuleCreate<Worlds>(UpdatePre, "worlds");
		ModuleCreate<Particles>(UpdateNormal, "particles");
		ModuleCreate<Terrains>(UpdateNormal, "terrains");
		ModuleCreate<Shadows>(UpdateNormal, "shadows");
		ModuleCreate<Waters>(UpdatePre, "waters");
	}

	void DefaultUpdater::Update()
	{
		if (Display::Get() != nullptr)
		{
			m_timerRender->SetInterval(1.0f / Display::Get()->GetFpsLimit());
		}

		// Always-Update.
		RunUpdate(UpdateAlways);

		if (m_timerUpdate->IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->ResetStartTime();

			// Pre-Update.
			RunUpdate(UpdatePre);

			// Update.
			RunUpdate(UpdateNormal);

			// Post-Update.
			RunUpdate(UpdatePost);

			// Updates the engines delta.
			m_deltaUpdate->Update();
		}

		// Prioritize updates over rendering.
		if (!Maths::AlmostEqual(m_timerUpdate->GetInterval(), m_deltaUpdate->GetChange(), 5.0f))
		{
			return;
		}

		// Renders when needed.
		if (m_timerRender->IsPassedTime() || Display::Get()->GetFpsLimit() <= 0.0f || Display::Get()->GetFpsLimit() > 1000.0f)
		{
			// Resets the timer.
			m_timerRender->ResetStartTime();

			// Render
			RunUpdate(UpdateRender);

			// Updates the render delta, and render time extension.
			m_deltaRender->Update();
		}
	}

	void DefaultUpdater::AddModule(const ModuleUpdate &typeUpdate, std::string moduleName, IModule *module)
	{
		float offset = typeUpdate + (0.01f * static_cast<float>(m_modules->size()));
		m_modules->insert(std::make_pair(offset, std::make_pair(moduleName, module)));
	}

	IModule *DefaultUpdater::GetModule(const std::string &name)
	{
		for (auto &module : *m_modules)
		{
			if (module.second.first == name)
			{
				return module.second.second;
			}
		}

		return nullptr;
	}

	void DefaultUpdater::RunUpdate(const ModuleUpdate &typeUpdate) const
	{
		for (auto &module : *m_modules)
		{
			if (static_cast<int>(std::floor(module.first)) == typeUpdate)
			{
				module.second.second->Update();
			}
		}
	}
}
