#include "ModuleManager.hpp"

#include "Audio/Audio.hpp"
#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Devices/Window.hpp"
#include "Events/Events.hpp"
#include "Files/Files.hpp"
#include "Gizmos/Gizmos.hpp"
#include "Log.hpp"
#include "Module.hpp"
#include "Particles/Particles.hpp"
#include "Renderer/Renderer.hpp"
#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
ModuleManager::ModuleManager() {}

ModuleManager::~ModuleManager()
{
	for(auto it = m_modules.rbegin(); it != m_modules.rend(); ++it)
		{
			it->second->~Module();
		}
}

void ModuleManager::FillRegister()
{
	Add<Window>(Module::Stage::Always);
	Add<Renderer>(Module::Stage::Render);
	Add<Audio>(Module::Stage::Pre);
	Add<Joysticks>(Module::Stage::Pre);
	Add<Keyboard>(Module::Stage::Pre);
	Add<Mouse>(Module::Stage::Pre);
	Add<Files>(Module::Stage::Pre);
	Add<Scenes>(Module::Stage::Normal);
	Add<Gizmos>(Module::Stage::Normal);
	Add<Resources>(Module::Stage::Pre);
	Add<Events>(Module::Stage::Always);
	Add<Uis>(Module::Stage::Pre);
	Add<Particles>(Module::Stage::Normal);
	Add<Shadows>(Module::Stage::Normal);
}

bool ModuleManager::Contains(Module* module)
{
	for(const auto& m : m_modules)
		{
			if(m.second.get() == module)
				{
					return true;
				}
		}

	return false;
}

Module* ModuleManager::Add(Module* module, const Module::Stage& update)
{
	if(Contains(module))
		{
			Log::Error("Module '%i' is already registered!\n", update);
			return nullptr;
		}

	float key = static_cast<float>(update) + (0.01f * static_cast<float>(m_modules.size()));
	m_modules.emplace(key, module);
	return module;
}

void ModuleManager::Remove(Module* module)
{
	for(auto it = --m_modules.end(); it != m_modules.begin(); --it) // TODO: Clean remove.
		{
			if((*it).second.get() != module)
				{
					continue;
				}

			m_modules.erase(it);
		}
}

void ModuleManager::RunUpdate(const Module::Stage& update)
{
	for(auto& [key, module] : m_modules)
		{
			if(static_cast<uint32_t>(std::floor(key)) == static_cast<uint32_t>(update))
				{
					module->Update();
				}
		}
}
}
