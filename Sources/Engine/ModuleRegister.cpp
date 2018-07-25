#include "ModuleRegister.hpp"

#include "Audio/Audio.hpp"
#include "Display/Display.hpp"
#include "Files/Files.hpp"
#include "Events/Events.hpp"
#include "Inputs/Joysticks.hpp"
#include "Inputs/Keyboard.hpp"
#include "Inputs/Mouse.hpp"
#include "Inputs/Touches.hpp"
#include "Particles/Particles.hpp"
#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	ModuleRegister::ModuleRegister() :
		m_modules(std::map<float, IModule *>())
	{
	}

	ModuleRegister::~ModuleRegister()
	{
		for (auto it = --m_modules.end(); it != m_modules.begin(); --it)
		{
			delete (*it).second;
		}
	}

	void ModuleRegister::FillRegister()
	{
		RegisterModule<Display>(UPDATE_POST);
		RegisterModule<Joysticks>(UPDATE_PRE);
		RegisterModule<Keyboard>(UPDATE_PRE);
		RegisterModule<Mouse>(UPDATE_PRE);
		RegisterModule<Touches>(UPDATE_PRE);
		RegisterModule<Audio>(UPDATE_PRE);
		RegisterModule<Files>(UPDATE_PRE);
		RegisterModule<Scenes>(UPDATE_NORMAL);
		RegisterModule<Renderer>(UPDATE_RENDER);
		RegisterModule<Resources>(UPDATE_PRE);
		RegisterModule<Tasks>(UPDATE_ALWAYS);
		RegisterModule<Events>(UPDATE_ALWAYS);
		RegisterModule<Uis>(UPDATE_PRE);
		RegisterModule<Particles>(UPDATE_NORMAL);
		RegisterModule<Shadows>(UPDATE_NORMAL);
	}

	IModule *ModuleRegister::RegisterModule(IModule *module, const ModuleUpdate &update)
	{
		if (ContainsModule(module))
		{
			fprintf(stderr, "Module '%i' is already registered!\n", update);
			return nullptr;
		}

		float offset = update + (0.01f * static_cast<float>(m_modules.size()));
		m_modules.emplace(offset, module);
		return module;
	}

	bool ModuleRegister::ContainsModule(IModule *module) const
	{
		for (auto &module1 : m_modules)
		{
			if (module1.second == module)
			{
				return true;
			}
		}

		return false;
	}

	IModule *ModuleRegister::DeregisterModule(IModule *module)
	{
		for (auto it = --m_modules.end(); it != m_modules.begin(); --it)
		{
			if ((*it).second != module)
			{
				continue;
			}

			m_modules.erase(it);
			delete (*it).second;
			return (*it).second;
		}

		return nullptr;
	}

	void ModuleRegister::RunUpdate(const ModuleUpdate &update) const
	{
		for (auto &module : m_modules)
		{
			if (static_cast<int>(std::floor(module.first)) == update)
			{
				module.second->Update();
			}
		}
	}
}
