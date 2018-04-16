#include "ModuleRegister.hpp"

#include "Audio/Audio.hpp"
#include "Display/Display.hpp"
#include "Events/Events.hpp"
#include "Inputs/Joysticks.hpp"
#include "Inputs/Keyboard.hpp"
#include "Inputs/Mouse.hpp"
#include "Particles/Particles.hpp"
#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Uis/Uis.hpp"
#include "Worlds/Worlds.hpp"

namespace Flounder
{
	ModuleRegister::ModuleRegister() :
		m_modules(new std::multimap<float, ModulePair>()),
		m_deltaUpdate(new Delta()),
		m_deltaRender(new Delta()),
		m_timerUpdate(new Timer(1.0f / 66.0f)),
		m_timerRender(new Timer(1.0f / -1.0f))
	{
		RegisterModule<Display>(UpdatePost, "display");
		RegisterModule<Joysticks>(UpdatePre, "joysticks");
		RegisterModule<Keyboard>(UpdatePre, "keyboard");
		RegisterModule<Mouse>(UpdatePre, "mouse");
		RegisterModule<Audio>(UpdatePre, "audio");
		RegisterModule<Scenes>(UpdateNormal, "scenes");
		RegisterModule<Renderer>(UpdateRender, "renderer");
		RegisterModule<Resources>(UpdatePre, "resources");
		RegisterModule<Events>(UpdatePre, "events");
		RegisterModule<Tasks>(UpdatePre, "tasks");
		RegisterModule<Uis>(UpdatePre, "uis");
		RegisterModule<Worlds>(UpdatePre, "worlds");
		RegisterModule<Particles>(UpdateNormal, "particles");
		RegisterModule<Shadows>(UpdateNormal, "shadows");
	}

	ModuleRegister::~ModuleRegister()
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

	void ModuleRegister::Update()
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

	IModule *ModuleRegister::GetModule(const std::string &name)
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

	void ModuleRegister::RunUpdate(const ModuleUpdate &typeUpdate) const
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
