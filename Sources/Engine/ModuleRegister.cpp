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

namespace fl
{
	ModuleRegister::ModuleRegister(const bool &emptyRegister) :
		m_modules(std::map<float, ModulePair>())
	{
		if (!emptyRegister)
		{
			RegisterModule<Display>(UPDATE_POST, "Display");
			RegisterModule<Joysticks>(UPDATE_PRE, "Joysticks");
			RegisterModule<Keyboard>(UPDATE_PRE, "Keyboard");
			RegisterModule<Mouse>(UPDATE_PRE, "Mouse");
			RegisterModule<Audio>(UPDATE_PRE, "Audio");
			RegisterModule<Files>(UPDATE_PRE, "Files");
			RegisterModule<Scenes>(UPDATE_NORMAL, "Scenes");
			RegisterModule<Renderer>(UPDATE_RENDER, "Renderer");
			RegisterModule<Resources>(UPDATE_PRE, "Resources");
			RegisterModule<Tasks>(UPDATE_ALWAYS, "Tasks");
			RegisterModule<Events>(UPDATE_ALWAYS, "Events");
			RegisterModule<Uis>(UPDATE_PRE, "Uis");
			RegisterModule<Particles>(UPDATE_NORMAL, "Particles");
			RegisterModule<Shadows>(UPDATE_NORMAL, "Shadows");
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
