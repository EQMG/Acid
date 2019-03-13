#include "ComponentRegister.hpp"

#include "Emitters/EmitterCircle.hpp"
#include "Emitters/EmitterLine.hpp"
#include "Emitters/EmitterPoint.hpp"
#include "Emitters/EmitterSphere.hpp"
#include "Lights/Light.hpp"
#include "Materials/MaterialDefault.hpp"
#include "Meshes/Mesh.hpp"
#include "Meshes/MeshRender.hpp"
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
ComponentRegister::ComponentRegister()
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
	Add<MeshRender>("MeshRender");
	Add<ParticleSystem>("ParticleSystem");
	Add<Rigidbody>("Rigidbody");
	Add<ShadowRender>("ShadowRender");
}

void ComponentRegister::Remove(const std::string& name)
{
	m_components.erase(name);
}

Component* ComponentRegister::Create(const std::string& name) const
{
	auto it = m_components.find(name);

	if(it == m_components.end())
		{
			Log::Error("Could not find registered component: '%s'\n", name.c_str());
			return nullptr;
		}

	return ((*it).second).m_create();
}

std::optional<std::string> ComponentRegister::FindName(Component* compare) const
{
	for(const auto& [name,
					 component] : m_components) // TODO: Clean remove.
		{
			if(component.m_isSame(compare))
				{
					return name;
				}
		}

	return {};
}
}
