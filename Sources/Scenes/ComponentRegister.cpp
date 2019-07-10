#include "ComponentRegister.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Emitters/EmitterCircle.hpp"
#include "Emitters/EmitterLine.hpp"
#include "Emitters/EmitterPoint.hpp"
#include "Emitters/EmitterSphere.hpp"
#include "Lights/Light.hpp"
#include "Materials/MaterialDefault.hpp"
#include "Maths/Transform.hpp"
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
	Add<ColliderCapsule>("colliderCapsule");
	Add<ColliderCone>("colliderCone");
	Add<ColliderConvexHull>("colliderConvexHull");
	Add<ColliderCube>("colliderCube");
	Add<ColliderCylinder>("colliderCylinder");
	Add<ColliderHeightfield>("colliderHeightfield");
	Add<ColliderSphere>("colliderSphere");
	Add<EmitterCircle>("emitterCircle");
	Add<EmitterLine>("emitterLine");
	Add<EmitterPoint>("emitterPoint");
	Add<EmitterSphere>("emitterSphere");
	Add<KinematicCharacter>("kinematicCharacter");
	Add<Light>("light");
	Add<MaterialDefault>("materialDefault");
	Add<MaterialSkybox>("materialSkybox");
	Add<Transform>("transform");
	Add<Mesh>("mesh");
	Add<MeshAnimated>("meshAnimated");
	Add<MeshRender>("meshRender");
	Add<ParticleSystem>("particleSystem");
	Add<Rigidbody>("rigidbody");
	Add<ShadowRender>("shadowRender");
}

void ComponentRegister::Remove(const std::string &name)
{
	m_components.erase(name);
}

Component *ComponentRegister::Create(const std::string &name) const
{
	auto it{m_components.find(name)};

	if (it == m_components.end())
	{
		std::cerr << "Could not find registered component: " << std::quoted(name) << '\n';
		return nullptr;
	}

	return ((*it).second).m_create();
}

void ComponentRegister::Decode(const std::string &name, const Metadata &metadata, Component *component)
{
	auto it{m_components.find(name)};

	if (it == m_components.end())
	{
		std::cerr << "Could not find registered component: " << std::quoted(name) << '\n';
		return;
	}

	((*it).second).m_decode(metadata, component);
}

void ComponentRegister::Encode(const std::string &name, Metadata &metadata, const Component *component)
{
	auto it{m_components.find(name)};

	if (it == m_components.end())
	{
		std::cerr << "Could not find registered component: " << std::quoted(name) << '\n';
		return;
	}

	((*it).second).m_encode(metadata, component);
}

std::optional<std::string> ComponentRegister::FindName(Component *compare) const
{
	for (const auto &[name, component] : m_components) // TODO: Clean remove.
	{
		if (component.m_isSame(compare))
		{
			return name;
		}
	}

	return std::nullopt;
}
}
