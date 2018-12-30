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
		m_mutex(std::mutex()),
		m_components(std::map<std::string, ComponentCreate>())
	{
		Add<ColliderCapsule>("ColliderCapsule");
		Add<ColliderCone>("ColliderCone");
		Add<ColliderConvexHull>("ColliderConvexHull");
		Add<ColliderCube>("ColliderCube");
		Add<ColliderCylinder>("ColliderCylinder");
		Add<ColliderHeightfield>("ColliderHeightfield");
		Add<ColliderSphere>("ColliderSphere");
		Add<EmitterCircle>("EmitterCircle");
		Add<EmitterLine>("EmitterLine");
		Add<EmitterPoint>("EmitterPoint");
		Add<EmitterSphere>("EmitterSphere");
		Add<KinematicCharacter>("KinematicCharacter");
		Add<Light>("Light");
		Add<MaterialDefault>("MaterialDefault");
		Add<MaterialSkybox>("MaterialSkybox");
		Add<Mesh>("Mesh");
		Add<MeshAnimated>("MeshAnimated");
		Add<MeshRender>("MeshRender");
		Add<ParticleSystem>("ParticleSystem");
		Add<Rigidbody>("Rigidbody");
		Add<ShadowRender>("ShadowRender");
	}

	void ComponentRegister::Remove(const std::string &name)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		//	m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [name](const std::string &n, const ComponentCreate &c){
		//		return name == n; // FIXME: Remove
		//	}), m_components.end());
	}

	Component *ComponentRegister::Create(const std::string &name) const
	{
		auto it = m_components.find(name);

		if (it == m_components.end())
		{
			Log::Error("Could not find registered component: '%s'\n", name.c_str());
			return nullptr;
		}

		return ((*it).second).m_create();
	}

	std::optional<std::string> ComponentRegister::FindName(Component *compare) const
	{
		//	auto found = std::find_if(m_components.begin(), m_components.end(), [compare](const std::string &s, const ComponentCreate &c){
		//		return c.m_isSame(compare);// FIXME: Remove
		//	});

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
