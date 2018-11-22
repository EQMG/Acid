#include "ComponentRegister.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Lights/Light.hpp"
#include "Materials/MaterialDefault.hpp"
#include "Meshes/MeshRender.hpp"
#include "Particles/ParticleSystem.hpp"
#include "Physics/ColliderCube.hpp"
#include "Physics/ColliderCapsule.hpp"
#include "Physics/CharacterController.hpp"
#include "Physics/ColliderCone.hpp"
#include "Physics/ColliderConvexHull.hpp"
#include "Physics/ColliderCylinder.hpp"
#include "Physics/ColliderHeightfield.hpp"
#include "Physics/ColliderSphere.hpp"
#include "Physics/Rigidbody.hpp"
#include "Shadows/ShadowRender.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace acid
{
	ComponentRegister::ComponentRegister() :
		m_components(std::map<std::string, ComponentCreate>())
	{
		RegisterComponent<Light>("Light");
		RegisterComponent<MaterialDefault>("MaterialDefault");
		RegisterComponent<MaterialSkybox>("MaterialSkybox");
		RegisterComponent<Mesh>("Mesh");
		RegisterComponent<MeshAnimated>("MeshAnimated");
		RegisterComponent<MeshRender>("MeshRender");
		RegisterComponent<ParticleSystem>("ParticleSystem");
		RegisterComponent<ColliderCube>("ColliderCube");
		RegisterComponent<ColliderCapsule>("ColliderCapsule");
		RegisterComponent<CharacterController>("CharacterController");
		RegisterComponent<ColliderCone>("ColliderCone");
		RegisterComponent<ColliderConvexHull>("ColliderConvexHull");
		RegisterComponent<ColliderCylinder>("ColliderCylinder");
		RegisterComponent<ColliderHeightfield>("ColliderHeightfield");
		RegisterComponent<ColliderSphere>("ColliderSphere");
		RegisterComponent<Rigidbody>("Rigidbody");
		RegisterComponent<ShadowRender>("ShadowRender");
	}

	bool ComponentRegister::DeregisterComponent(const std::string &name)
	{
		auto component = m_components.find(name);

		if (component == m_components.end())
		{
			return false;
		}

		m_components.erase(component);
		return true;
	}

	IComponent *ComponentRegister::CreateComponent(const std::string &name)
	{
		auto it = m_components.find(name);

		if (it == m_components.end())
		{
			Log::Error("Could not find registered component: '%s'\n", name.c_str());
			return nullptr;
		}

		return ((*it).second).m_create();
	}

	std::optional<std::string> ComponentRegister::FindComponentName(IComponent *compare)
	{
		for (auto &[name, component] : m_components)
		{
			if (component.m_isSame(compare))
			{
				return name;
			}
		}

		return {};
	}
}
