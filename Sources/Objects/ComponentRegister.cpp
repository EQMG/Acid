#include "ComponentRegister.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Lights/Light.hpp"
#include "Materials/MaterialDefault.hpp"
#include "Meshes/MeshRender.hpp"
#include "Particles/ParticleSystem.hpp"
#include "Physics/ColliderSphere.hpp"
#include "Physics/Rigidbody.hpp"
#include "Shadows/ShadowRender.hpp"
#include "Skyboxes/CelestialBody.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace fl
{
	ComponentRegister::ComponentRegister() :
		m_components(new std::map<std::string, ComponentCreate *>())
	{
		RegisterComponent<CelestialBody>("CelestialBody");
		RegisterComponent<ColliderAabb>("AabbCollider");
		RegisterComponent<ColliderSphere>("SphereCollider");
		RegisterComponent<Light>("Light");
		RegisterComponent<MaterialDefault>("MaterialDefault");
		RegisterComponent<MaterialSkybox>("MaterialSkybox");
		RegisterComponent<Mesh>("Mesh");
		RegisterComponent<MeshAnimated>("MeshAnimated");
		RegisterComponent<MeshRender>("MeshRender");
		RegisterComponent<ParticleSystem>("ParticleSystem");
		RegisterComponent<Rigidbody>("Rigidbody");
		RegisterComponent<ShadowRender>("ShadowRender");
	}

	ComponentRegister::~ComponentRegister()
	{
		for (auto it = --m_components->end(); it != m_components->begin(); --it)
		{
			delete (*it).second;
		}

		delete m_components;
	}

	ComponentRegister::ComponentCreate *ComponentRegister::GetComponent(const std::string &name)
	{
		for (auto &component : *m_components)
		{
			if (component.first == name)
			{
				return component.second;
			}
		}

		return nullptr;
	}

	void ComponentRegister::DeregisterComponent(const std::string &name)
	{
		for (auto it = --m_components->end(); it != m_components->begin(); --it)
		{
			m_components->erase(it);
			delete (*it).second;
		}
	}

	Component *ComponentRegister::CreateComponent(const std::string &name)
	{
		auto found = m_components->find(name);

		if (found == m_components->end())
		{
			fprintf(stderr, "Could not find registered component: '%s'\n", name.c_str());
			return nullptr;
		}

		return (*(*found).second)();
	}
}
