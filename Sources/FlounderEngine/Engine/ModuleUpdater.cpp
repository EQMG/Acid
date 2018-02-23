#include <Objects/ComponentRegister.hpp>
#include "ModuleUpdater.hpp"

#include "../Devices/Audio.hpp"
#include "../Devices/Joysticks.hpp"
#include "../Devices/Keyboard.hpp"
#include "../Devices/Mouse.hpp"
#include "../Scenes/Scenes.hpp"
#include "../Events/Events.hpp"
#include "../Particles/Particles.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Shadows/Shadows.hpp"
#include "../Skyboxes/Skyboxes.hpp"
#include "../Terrains/Terrains.hpp"
#include "../Uis/Uis.hpp"
#include "../Waters/Waters.hpp"
#include "../Worlds/Worlds.hpp"

#include "../Skyboxes/CelestialBody.hpp"
#include "../Entities/EntityRender.hpp"
#include "../Lights/Light.hpp"
#include "../Physics/Aabb.hpp"
#include "../Physics/Sphere.hpp"
#include "../Materials/Material.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Physics/Rigidbody.hpp"
#include "../Skyboxes/SkyboxRender.hpp"
#include "../Terrains/TerrainRender.hpp"
#include "../Waters/WaterRender.hpp"

namespace Flounder
{
	ModuleUpdater::ModuleUpdater() :
		IUpdater(),
		m_deltaUpdate(nullptr),
		m_deltaRender(nullptr),
		m_timerUpdate(nullptr),
		m_timerRender(nullptr),
		m_modules(new std::multimap<float, std::pair<std::string, IModule *>>())
	{
	}

	ModuleUpdater::~ModuleUpdater()
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

	void ModuleUpdater::Create()
	{
		m_deltaUpdate = new Delta();
		m_deltaRender = new Delta();
		m_timerUpdate = new Timer(1.0f / 62.0f);
		m_timerRender = new Timer(1.0f / -1.0f);

		ComponentRegister::Register("CelestialBody", REGISTER_CREATE(CelestialBody));
		ComponentRegister::Register("EntityRender", REGISTER_CREATE(EntityRender));
		ComponentRegister::Register("Light", REGISTER_CREATE(Light));
		ComponentRegister::Register("AabbCollider", REGISTER_CREATE(Aabb));
		ComponentRegister::Register("SphereCollider", REGISTER_CREATE(Sphere));
		ComponentRegister::Register("Material", REGISTER_CREATE(Material));
		ComponentRegister::Register("Mesh", REGISTER_CREATE(Mesh));
		ComponentRegister::Register("Rigidbody", REGISTER_CREATE(Rigidbody));
		ComponentRegister::Register("SkyboxRender", REGISTER_CREATE(SkyboxRender));
		ComponentRegister::Register("TerrainRender", REGISTER_CREATE(TerrainRender));
		ComponentRegister::Register("WaterRender", REGISTER_CREATE(WaterRender));

		ModuleCreate<Audio>(UpdatePre, "audio");
		ModuleCreate<Display>(UpdatePost, "display");
		ModuleCreate<Joysticks>(UpdatePre, "joysticks");
		ModuleCreate<Keyboard>(UpdatePre, "keyboard");
		ModuleCreate<Mouse>(UpdatePre, "mouse");
		ModuleCreate<Renderer>(UpdateRender, "renderer");
		ModuleCreate<Resources>(UpdatePre, "resources");
		ModuleCreate<Events>(UpdatePre, "events");
		ModuleCreate<Tasks>(UpdatePre, "tasks");
		ModuleCreate<Uis>(UpdatePre, "uis");
		ModuleCreate<Worlds>(UpdatePre, "worlds");
		ModuleCreate<Scenes>(UpdateNormal, "scenes");
		ModuleCreate<Particles>(UpdateNormal, "particles");
		ModuleCreate<Skyboxes>(UpdateNormal, "skyboxes");
		ModuleCreate<Terrains>(UpdateNormal, "terrains");
		ModuleCreate<Shadows>(UpdateNormal, "shadows");
		ModuleCreate<Waters>(UpdatePre, "waters");
	}

	void ModuleUpdater::Update()
	{
		if (Display::Get() != nullptr)
		{
			m_timerRender->SetInterval(1.0f / Display::Get()->GetFpsLimit());
		}

		// Always-Update
		RunUpdate(UpdateAlways);

		if (m_timerUpdate->IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->ResetStartTime();

			// Updates the engines delta.
			m_deltaUpdate->Update();

			// Pre-Update
			RunUpdate(UpdatePre);

			// Update
			RunUpdate(UpdateNormal);

			// Post-Update
			RunUpdate(UpdatePost);
		}

		// Renders when needed.
		if (m_timerRender->IsPassedTime() || Display::Get()->GetFpsLimit() <= 0.0f || Display::Get()->GetFpsLimit() > 1000.0f)
		{
			// if (maths::almostEqual(m_timerUpdate->getInterval(), m_deltaUpdate->getChange(), 7.0f)) {}

			// Resets the timer.
			m_timerRender->ResetStartTime();

			// Updates the render delta, and render time extension.
			m_deltaRender->Update();

			// Render
			RunUpdate(UpdateRender);
		}
	}

	void ModuleUpdater::AddModule(const ModuleUpdate &typeUpdate, std::string moduleName, IModule *module)
	{
		float offset = typeUpdate + (0.01f * static_cast<float>(m_modules->size()));
		m_modules->insert(std::make_pair(offset, std::make_pair(moduleName, module)));
	}

	IModule *ModuleUpdater::GetModule(const std::string &name)
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

	void ModuleUpdater::RunUpdate(const ModuleUpdate &typeUpdate) const
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
