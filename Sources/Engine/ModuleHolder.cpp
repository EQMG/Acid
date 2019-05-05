#include "ModuleHolder.hpp"

#include "Audio/Audio.hpp"
#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Devices/Window.hpp"
#include "Files/Files.hpp"
#include "Gizmos/Gizmos.hpp"
#include "Particles/Particles.hpp"
#include "Renderer/Renderer.hpp"
#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Uis/Uis.hpp"
#include "Log.hpp"
#include "Module.hpp"

namespace acid
{
void ModuleHolder::FillRegister()
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
	Add<Uis>(Module::Stage::Pre);
	Add<Particles>(Module::Stage::Normal);
	Add<Shadows>(Module::Stage::Normal);
}

void ModuleHolder::RemoveModuleStage(const TypeId &id)
{
	for (auto it = m_stages.begin(); it != m_stages.end();)
	{
		if (it->second == id)
		{
			it = m_stages.erase(it);
		}
		else
		{
			++it;
		}
	}

}
}
