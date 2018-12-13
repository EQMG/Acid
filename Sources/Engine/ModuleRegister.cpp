#include "ModuleRegister.hpp"

#include "Log.hpp"
#include "Audio/Audio.hpp"
#include "Display/Display.hpp"
#include "Events/Events.hpp"
#include "Files/Files.hpp"
#include "Gizmos/Gizmos.hpp"
#include "Inputs/Joysticks.hpp"
#include "Inputs/Keyboard.hpp"
#include "Inputs/Mouse.hpp"
#include "Particles/Particles.hpp"
#include "Renderer/Renderer.hpp"
#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	ModuleRegister::ModuleRegister() :
		m_modules(std::map<float, std::unique_ptr<IModule>>())
	{
	}

	void ModuleRegister::FillRegister()
	{
		RegisterModule<Display>(MODULE_UPDATE_POST);
		RegisterModule<Joysticks>(MODULE_UPDATE_PRE);
		RegisterModule<Keyboard>(MODULE_UPDATE_PRE);
		RegisterModule<Mouse>(MODULE_UPDATE_PRE);
		RegisterModule<Audio>(MODULE_UPDATE_PRE);
		RegisterModule<Files>(MODULE_UPDATE_PRE);
		RegisterModule<Scenes>(MODULE_UPDATE_NORMAL);
		RegisterModule<Gizmos>(MODULE_UPDATE_NORMAL);
		RegisterModule<Renderer>(MODULE_UPDATE_RENDER);
		RegisterModule<Resources>(MODULE_UPDATE_PRE);
		RegisterModule<Events>(MODULE_UPDATE_ALWAYS);
		RegisterModule<Uis>(MODULE_UPDATE_PRE);
		RegisterModule<Particles>(MODULE_UPDATE_NORMAL);
		RegisterModule<Shadows>(MODULE_UPDATE_NORMAL);
	}

	IModule *ModuleRegister::RegisterModule(IModule *module, const ModuleUpdate &update)
	{
		if (ContainsModule(module))
		{
			Log::Error("Module '%i' is already registered!\n", update);
			return nullptr;
		}

		float key = static_cast<float>(update) + (0.01f * static_cast<float>(m_modules.size()));
		m_modules.emplace(key, module);
		return module;
	}

	bool ModuleRegister::ContainsModule(IModule *module) const
	{
		for (auto &module1 : m_modules)
		{
			if (module1.second.get() == module)
			{
				return true;
			}
		}

		return false;
	}

	bool ModuleRegister::DeregisterModule(IModule *module)
	{
		for (auto it = --m_modules.end(); it != m_modules.begin(); --it)
		{
			if ((*it).second.get() != module)
			{
				continue;
			}

			m_modules.erase(it);
			return true;
		}

		return false;
	}

	void ModuleRegister::RunUpdate(const ModuleUpdate &update) const
	{
		for (auto &[key, module] : m_modules)
		{
			if (static_cast<int32_t>(std::floor(key)) == update)
			{
				module->Update();
			}
		}
	}
}
