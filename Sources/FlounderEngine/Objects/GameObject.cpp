#include "GameObject.hpp"

#include "Prefabs/PrefabObject.hpp"
#include "../Scenes/Scenes.hpp"
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
	GameObject::GameObject(const Transform &transform, ISpatialStructure<GameObject *> *structure) :
		m_name("Unnamed"),
		m_transform(new Transform(transform)),
		m_components(new std::vector<Component*>()),
		m_structure(structure),
		m_removed(false)
	{
		if (m_structure == nullptr)
		{
			m_structure = Scenes::Get()->GetStructure();
		}

		if (m_structure != nullptr)
		{
			m_structure->Add(this);
		}
	}

	GameObject::GameObject(const std::string &prefabName, const Transform &transform, ISpatialStructure<GameObject *> *structure) :
		GameObject(transform, structure)
	{
		PrefabObject *entityPrefab = PrefabObject::Resource("Resources/Entities/" + prefabName + "/" + prefabName + ".csv");
		m_name = prefabName;

		for (const auto &componentName : entityPrefab->GetComponents())
		{
			if (FormatString::StartsWith(componentName, "#"))
			{
				continue;
			}

			Component *component = CreateComponent(componentName);

			if (component == nullptr)
			{
				continue;
			}

			PrefabComponent *componentPrefab = entityPrefab->GetComponentData(componentName);

			if (componentPrefab == nullptr)
			{
				continue;
			}

			component->Load(componentPrefab);
			AddComponent(component);
		}
	}

	GameObject::~GameObject()
	{
		StructureRemove();

		for (auto component : *m_components)
		{
			delete component;
		}

		delete m_components;
		delete m_transform;
	}

	void GameObject::Update()
	{
		for (auto c : *m_components)
		{
			c->Update();
		}
	}

	void GameObject::AddComponent(Component *component)
	{
		m_components->push_back(component);
		component->SetGameObject(this);
	}

	void GameObject::RemoveComponent(Component *component)
	{
		for (auto it = m_components->begin(); it != m_components->end(); ++it)
		{
			if (*it == component)
			{
				component->SetGameObject(nullptr);
				delete component;
				m_components->erase(it);
				return;
			}
		}
	}

	void GameObject::StructureSwitch(ISpatialStructure<GameObject *> *structure)
	{
		if (m_structure != nullptr)
		{
			m_structure->Remove(this);
		}

		structure->Add(this);
		m_structure = structure;
	}

	void GameObject::StructureRemove()
	{
		if (m_structure != nullptr)
		{
			m_structure->Remove(this);
			m_structure = nullptr;
		}

		m_removed = true;
	}

	Component *GameObject::CreateComponent(const std::string &name)
	{
		if (name == "CelestialBody") { return new CelestialBody(); }
		if (name == "ColliderAabb") { return new Aabb(); }
		if (name == "ColliderSphere") { return new Sphere(); }
		if (name == "EntityRender") { return new EntityRender(); }
		if (name == "Light") { return new Light(); }
		if (name == "Material") { return new Material(); }
		if (name == "Mesh") { return new Mesh(); }
		if (name == "Rigidbody") { return new Rigidbody(); }
		if (name == "SkyboxRender") { return new SkyboxRender(); }
		if (name == "TerrainRender") { return new TerrainRender(); }
		if (name == "WaterRender") { return new WaterRender(); }
		return nullptr;
	}
}
