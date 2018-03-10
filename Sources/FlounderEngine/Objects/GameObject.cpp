#include "GameObject.hpp"

#include "Prefabs/PrefabObject.hpp"
#include "../Scenes/Scenes.hpp"
#include "ComponentRegister.hpp"

namespace Flounder
{
	GameObject::GameObject(const Transform &transform, ISpatialStructure<GameObject *> *structure, const std::string &name) :
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
		PrefabObject *entityPrefab = PrefabObject::Resource("Resources/Entities/" + prefabName + "/" + prefabName + ".json");

		for (auto value : entityPrefab->GetParent()->m_children)
		{
			if (value->m_name.empty())
			{
				continue;
			}

			Component *component = ComponentRegister::Create(value->m_name);

			if (component == nullptr)
			{
				continue;
			}

			component->Load(value);
			AddComponent(component);
		}

	//	if (prefabName == "Testing")
	//	{
	//		entityPrefab->Write(this);
	//		entityPrefab->Save();
	//	}
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

			(*it)->Update();
		}
	}

	void GameObject::AddComponent(Component *component)
	{
		if (component == nullptr)
		{
			return;
		}

		component->SetGameObject(this);
		m_components->push_back(component);
	}

	void GameObject::RemoveComponent(Component *component)
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
