#include "GameObject.hpp"

#include "Scenes/Prefabs/PrefabObject.hpp"
#include "Scenes/Scenes.hpp"

namespace fl
{
	GameObject::GameObject(const Transform &transform, ISpatialStructure *structure, const std::string &name) :
		m_name(name),
		m_transform(new Transform(transform)),
		m_components(new std::vector<Component *>()),
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

	GameObject::GameObject(const std::string &prefabName, const Transform &transform, ISpatialStructure *structure) :
		GameObject(transform, structure, prefabName)
	{
		PrefabObject *entityPrefab = PrefabObject::Resource("Resources/Entities/" + prefabName + "/" + prefabName + ".json");

		for (auto value : *entityPrefab->GetParent()->GetChildren())
		{
			if (value->GetName().empty())
			{
				continue;
			}

			Component *component = Scenes::Get()->CreateComponent(value->GetName());

			if (component == nullptr)
			{
				continue;
			}

			component->Load(value);
			AddComponent(component);
		}

		if (prefabName == "Testing")
		{
			//	entityPrefab->Write(this);
			entityPrefab->Save();
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

		for (auto it = m_components->begin(); it != m_components->end(); ++it)
		{
			if ((*it) == nullptr || (*it)->GetGameObject() == nullptr)
			{
				continue;
			}

			if ((*it)->IsEnabled())
			{
				(*it)->Update();
			}
		}
	}

	Component *GameObject::AddComponent(Component *component)
	{
		if (component == nullptr)
		{
			return nullptr;
		}

		component->SetGameObject(this);
		m_components->push_back(component);
		return component;
	}

	Component *GameObject::RemoveComponent(Component *component)
	{
		for (auto it = m_components->begin(); it != m_components->end(); ++it)
		{
			if (*it == component)
			{
				if (*it != nullptr)
				{
					(*it)->SetGameObject(nullptr);
				}

				delete component;
				m_components->erase(it);
				return *it;
			}
		}

		return nullptr;
	}

	void GameObject::StructureSwitch(ISpatialStructure *structure)
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
