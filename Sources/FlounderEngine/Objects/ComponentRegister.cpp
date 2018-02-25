#include "ComponentRegister.hpp"

#include "../Skyboxes/CelestialBody.hpp"
#include "../Entities/EntityRender.hpp"
#include "../Lights/Light.hpp"
#include "../Physics/ColliderAabb.hpp"
#include "../Physics/ColliderSphere.hpp"
#include "../Physics/Rigidbody.hpp"
#include "../Particles/ParticleSystem.hpp"
#include "../Materials/Material.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Skyboxes/SkyboxRender.hpp"
#include "../Terrains/TerrainRender.hpp"
#include "../Waters/WaterRender.hpp"

namespace Flounder
{
	std::map<std::string, std::function<Component*()>*> ComponentRegister::VALUES = {
		std::make_pair("CelestialBody", REGISTER_CREATE(CelestialBody)),
		std::make_pair("EntityRender", REGISTER_CREATE(EntityRender)),
		std::make_pair("Light", REGISTER_CREATE(Light)),
		std::make_pair("AabbCollider", REGISTER_CREATE(ColliderAabb)),
		std::make_pair("SphereCollider", REGISTER_CREATE(ColliderSphere)),
		std::make_pair("ParticleSystem", REGISTER_CREATE(ParticleSystem)),
		std::make_pair("Mesh", REGISTER_CREATE(Mesh)),
		std::make_pair("Material", REGISTER_CREATE(Material)),
		std::make_pair("Rigidbody", REGISTER_CREATE(Rigidbody)),
		std::make_pair("SkyboxRender", REGISTER_CREATE(SkyboxRender)),
		std::make_pair("TerrainRender", REGISTER_CREATE(TerrainRender)),
		std::make_pair("WaterRender", REGISTER_CREATE(WaterRender))
	};

	void ComponentRegister::Register(const std::string &name, std::function<Component *()> *create)
	{
		if (VALUES.find(name) != VALUES.end())
		{
			fprintf(stderr, "Component '%s' is already registered!\n", name.c_str());
			return;
		}

		VALUES.insert(std::make_pair(name, create));
	}

	Component *ComponentRegister::Create(const std::string &name)
	{
		auto found = VALUES.find(name);

		if (found == VALUES.end())
		{
			fprintf(stderr, "Could not find registered component: '%s'\n", name.c_str());
			return nullptr;
		}

		return (*(*found).second)();
	}
}
