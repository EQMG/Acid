#include "GameObject.hpp"

#include "Prefabs/PrefabObject.hpp"
#include "../Scenes/Scenes.hpp"
#include "ComponentRegister.hpp"

namespace Flounder
{
	GameObject::GameObject(const Transform &transform, ISpatialStructure<GameObject *> *structure, std::string name) :
		m_name(name),
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
		GameObject(transform, structure, prefabName)
	{
		PrefabObject *entityPrefab = PrefabObject::Resource("Resources/Entities/" + prefabName + "/" + prefabName + ".csv");

		for (const auto &componentName : entityPrefab->GetComponents())
		{
			if (FormatString::StartsWith(componentName, "#"))
			{
				continue;
			}

			Component *component = ComponentRegister::Create(componentName);

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
		if (m_removed)
		{
			return;
		}

		for (auto c : *m_components)
		{
			if (c == nullptr || c->GetGameObject() == nullptr)
			{
				continue;
			}

			c->Update();
		}
	}

	void GameObject::AddComponent(Component *component)
	{
		component->SetGameObject(this);
		m_components->push_back(component);
	}

	void GameObject::RemoveComponent(Component *component)
	{
		for (auto it = m_components->begin(); it != m_components->end(); ++it)
		{
			if (*it == component)
			{
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
}
