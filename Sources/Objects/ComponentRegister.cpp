#include "ComponentRegister.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Lights/Light.hpp"
#include "Materials/MaterialDefault.hpp"
#include "Meshes/MeshRender.hpp"
#include "Particles/Emitters/EmitterCircle.hpp"
#include "Particles/Emitters/EmitterLine.hpp"
#include "Particles/Emitters/EmitterPoint.hpp"
#include "Particles/Emitters/EmitterSphere.hpp"
#include "Particles/ParticleSystem.hpp"
#include "Physics/Colliders/ColliderCapsule.hpp"
#include "Physics/Colliders/ColliderCone.hpp"
#include "Physics/Colliders/ColliderConvexHull.hpp"
#include "Physics/Colliders/ColliderCube.hpp"
#include "Physics/Colliders/ColliderCylinder.hpp"
#include "Physics/Colliders/ColliderHeightfield.hpp"
#include "Physics/Colliders/ColliderSphere.hpp"
#include "Physics/KinematicCharacter.hpp"
#include "Physics/Rigidbody.hpp"
#include "Shadows/ShadowRender.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace acid
{
	ComponentRegister::ComponentRegister() :
		m_components(std::map<std::string, ComponentCreate>())
	{
		RegisterComponent<ColliderCapsule>("ColliderCapsule");
		RegisterComponent<ColliderCone>("ColliderCone");
		RegisterComponent<ColliderConvexHull>("ColliderConvexHull");
		RegisterComponent<ColliderCube>("ColliderCube");
		RegisterComponent<ColliderCylinder>("ColliderCylinder");
		RegisterComponent<ColliderHeightfield>("ColliderHeightfield");
		RegisterComponent<ColliderSphere>("ColliderSphere");
		RegisterComponent<EmitterCircle>("EmitterCircle");
		RegisterComponent<EmitterLine>("EmitterLine");
		RegisterComponent<EmitterPoint>("EmitterPoint");
		RegisterComponent<EmitterSphere>("EmitterSphere");
		RegisterComponent<KinematicCharacter>("KinematicCharacter");
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

	Component *ComponentRegister::CreateComponent(const std::string &name)
	{
		auto it = m_components.find(name);

		if (it == m_components.end())
		{
			Log::Error("Could not find registered component: '%s'\n", name.c_str());
			return nullptr;
		}

		return ((*it).second).m_create();
	}

	std::optional<std::string> ComponentRegister::FindComponentName(Component *compare)
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
