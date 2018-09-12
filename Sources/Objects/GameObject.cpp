#include "GameObject.hpp"

#include "Helpers/FileSystem.hpp"
#include "Prefabs/PrefabObject.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	GameObject::GameObject(const Transform &transform, ISpatialStructure *structure) :
		m_name(""),
		m_transform(transform),
		m_components(std::vector<std::unique_ptr<IComponent>>()),
		m_structure(structure),
		m_parent(nullptr)
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

			component->Decode(*value);
			AddComponent(component);
		}

		m_name = FileSystem::FindName(filepath);
	}

	void GameObject::Update()
	{
		for (auto &component : m_components)
		{
			if (component->GetGameObject() == nullptr)
			{
				component->SetGameObject(this);
			}

			if (!component->IsStarted())
			{
				component->Start();
				component->SetStarted(true);
			}

			if (component->IsEnabled())
			{
				component->Update();
			}
		}
	}

	IComponent *GameObject::AddComponent(IComponent *component)
	{
		if (component == nullptr)
		{
			return nullptr;
		}

		component->SetGameObject(this);
		m_components.emplace_back(component);
		return component;
	}

	bool GameObject::RemoveComponent(IComponent *component)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it).get() == component)
			{
				(*it)->SetGameObject(nullptr);

				m_components.erase(it);
				return true;
			}
		}

		return false;
	}

	bool GameObject::RemoveComponent(const std::string &name)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			auto componentName = Scenes::Get()->FindComponentName((*it).get());

			if (componentName && name == *componentName)
			{
				continue;
			}

			(*it)->SetGameObject(nullptr);

			m_components.erase(it);
			return true;
		}

		return false;
	}

	void GameObject::SetStructure(ISpatialStructure *structure)
	{
		if (m_structure != nullptr)
		{
			m_structure->Remove(this);
		}

		m_structure = structure;
		m_structure->Add(this);
	}

	void GameObject::StructureRemove()
	{
		if (m_structure == nullptr)
		{
			return;
		}

		m_structure->Remove(this);
		m_structure = nullptr;
	}
}
