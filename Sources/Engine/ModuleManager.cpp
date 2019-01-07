#include "ModuleManager.hpp"

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
	ModuleManager::ModuleManager() :
		m_mutex(std::mutex()),
		m_modules(std::map<float, std::unique_ptr<Module>>())
	{
	}

	void ModuleManager::FillRegister()
	{
		Add<Display>(MODULE_UPDATE_POST);
		Add<Joysticks>(MODULE_UPDATE_PRE);
		Add<Keyboard>(MODULE_UPDATE_PRE);
		Add<Mouse>(MODULE_UPDATE_PRE);
		Add<Audio>(MODULE_UPDATE_PRE);
		Add<Files>(MODULE_UPDATE_PRE);
		Add<Scenes>(MODULE_UPDATE_NORMAL);
		Add<Gizmos>(MODULE_UPDATE_NORMAL);
		Add<Renderer>(MODULE_UPDATE_RENDER);
		Add<Resources>(MODULE_UPDATE_PRE);
		Add<Events>(MODULE_UPDATE_ALWAYS);
		Add<Uis>(MODULE_UPDATE_PRE);
		Add<Particles>(MODULE_UPDATE_NORMAL);
		Add<Shadows>(MODULE_UPDATE_NORMAL);
	}

	bool ModuleManager::Contains(Module *module)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto &module1 : m_modules)
		{
			if (module1.second.get() == module)
			{
				return true;
			}
		}

		return false;
	}

	Module *ModuleManager::Add(Module *module, const ModuleUpdate &update)
	{
		if (Contains(module))
		{
			Log::Error("Module '%i' is already registered!\n", update);
			return nullptr;
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		float key = static_cast<float>(update) + (0.01f * static_cast<float>(m_modules.size()));
		m_modules.emplace(key, module);
		return module;
	}

	void ModuleManager::Remove(Module *module)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto it = --m_modules.end(); it != m_modules.begin(); --it) // TODO: Clean remove.
		{
			if ((*it).second.get() != module)
			{
				continue;
			}

			m_modules.erase(it);
		}
	}

	void ModuleManager::RunUpdate(const ModuleUpdate &update)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto &[key, module] : m_modules)
		{
			if (static_cast<int32_t>(std::floor(key)) == update)
			{
				module->Update();
			}
		}
	}
}
