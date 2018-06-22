#include "ModuleRegister.hpp"

#include "Audio/Audio.hpp"
#include "Display/Display.hpp"
#include "Files/Files.hpp"
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

namespace fl
{
	ModuleRegister::ModuleRegister(const bool &emptyRegister) :
		m_modules(std::map<float, ModulePair>())
	{
		if (!emptyRegister)
		{
			RegisterModule<Display>(UPDATE_POST, "display");
			RegisterModule<Joysticks>(UPDATE_PRE, "joysticks");
			RegisterModule<Keyboard>(UPDATE_PRE, "keyboard");
			RegisterModule<Mouse>(UPDATE_PRE, "mouse");
			RegisterModule<Audio>(UPDATE_PRE, "audio");
			RegisterModule<Files>(UPDATE_PRE, "files");
			RegisterModule<Scenes>(UPDATE_NORMAL, "scenes");
			RegisterModule<Renderer>(UPDATE_RENDER, "renderer");
			RegisterModule<Resources>(UPDATE_PRE, "resources");
			RegisterModule<Tasks>(UPDATE_ALWAYS, "tasks");
			RegisterModule<Events>(UPDATE_ALWAYS, "events");
			RegisterModule<Uis>(UPDATE_PRE, "uis");
			RegisterModule<Worlds>(UPDATE_PRE, "worlds");
			RegisterModule<Particles>(UPDATE_NORMAL, "particles");
			RegisterModule<Shadows>(UPDATE_NORMAL, "shadows");
		}
	}

	ModuleRegister::~ModuleRegister()
	{
		for (auto it = --m_modules.end(); it != m_modules.begin(); --it)
		{
			delete (*it).second.second;
		}
	}

	IModule *ModuleRegister::RegisterModule(IModule *module, const ModuleUpdate &update, const std::string &name)
	{
		//	if (m_modules.find(name) != m_modules.end())
		//	{
		//		fprintf(stderr, "Module '%s' is already registered!\n", name.c_str());
		//		return;
		//	}

		float offset = update + (0.01f * static_cast<float>(m_modules.size()));
		m_modules.emplace(offset, std::make_pair(name, module));
		return module;
	}

	IModule *ModuleRegister::GetModule(const std::string &name)
	{
		for (auto &module : m_modules)
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
		for (auto it = --m_modules.end(); it != m_modules.begin(); --it)
		{
			m_modules.erase(it);
			delete (*it).second.second;
		}
	}

	void ModuleRegister::RunUpdate(const ModuleUpdate &update) const
	{
		for (auto &module : m_modules)
		{
			if (static_cast<int>(std::floor(module.first)) == update)
			{
				module.second.second->Update();
			}
		}
	}
}
