#include "GameObject.hpp"

#include "Helpers/FileSystem.hpp"
#include "Scenes/Scenes.hpp"
#include "Prefabs/PrefabObject.hpp"

namespace fl
{
	GameObject::GameObject(const Transform &transform, ISpatialStructure *structure) :
		m_name(""),
		m_transform(Transform(transform)),
		m_components(std::vector<std::shared_ptr<IComponent>>()),
		m_structure(structure),
		m_parent(nullptr),
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

	GameObject::GameObject(const std::string &filepath, const Transform &transform, ISpatialStructure *structure) :
		GameObject(transform, structure)
	{
		auto prefabObject = PrefabObject::Resource(filepath);

		for (auto &value : prefabObject->GetParent()->GetChildren())
		{
			if (value->GetName().empty())
			{
				continue;
			}

			auto component = Scenes::Get()->CreateComponent(value->GetName());

			if (component == nullptr)
			{
				continue;
			}

			component->Load(value);
			AddComponent(component);
		}

		m_name = FileSystem::FindName(filepath);
	}

	GameObject::~GameObject()
	{
		StructureRemove();
	}

	void GameObject::Update()
	{
		if (m_removed)
		{
			return;
		}

		for (auto it = m_components.begin(); it != m_components.end(); ++it)
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

	std::shared_ptr<IComponent> GameObject::AddComponent(std::shared_ptr<IComponent> component)
	{
		if (component == nullptr)
		{
			return nullptr;
		}

		component->SetGameObject(this);
		m_components.emplace_back(component);
		return component;
	}

	std::shared_ptr<IComponent> GameObject::RemoveComponent(std::shared_ptr<IComponent> component)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if (*it == component)
			{
				if (*it != nullptr)
				{
					(*it)->SetGameObject(nullptr);
				}

				m_components.erase(it);
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
