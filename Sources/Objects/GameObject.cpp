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
		m_parent(nullptr),
		m_removed(false)
	{
		if (structure == nullptr)
		{
			structure = Scenes::Get()->GetStructure();
		}

		if (structure != nullptr)
		{
			structure->Add(this);
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

	GameObject::~GameObject()
	{
	}

	void GameObject::Update()
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it) == nullptr || (*it)->GetGameObject() == nullptr)
			{
				continue;
			}

			if (!(*it)->IsStarted())
			{
				(*it)->Start();
				(*it)->SetStarted(true);
			}

			if ((*it)->IsEnabled())
			{
				(*it)->Update();
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
			if (*it != nullptr && (*it).get() == component)
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
			if (*it != nullptr)
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
		}

		return false;
	}
}
