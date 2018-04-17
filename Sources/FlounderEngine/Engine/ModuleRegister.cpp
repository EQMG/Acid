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
		m_modules(new std::multimap<float, ModulePair>())
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

	void ModuleRegister::DeregisterModule(const std::string &name)
	{
		for (auto it = --m_modules->end(); it != m_modules->begin(); --it)
		{
			m_modules->erase(it);
			delete (*it).second.second;
		}
	}

	void ModuleRegister::RunUpdate(const ModuleUpdate &update) const
	{
		for (auto &module : *m_modules)
		{
			if (static_cast<int>(std::floor(module.first)) == update)
			{
				module.second.second->Update();
			}
		}
	}
}
