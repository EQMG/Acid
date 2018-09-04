#include "GameObject.hpp"

#include "Helpers/FileSystem.hpp"
#include "Prefabs/PrefabObject.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	std::shared_ptr<GameObject> GameObject::Resource(const Transform &transform, ISpatialStructure *structure)
	{
		if (structure == nullptr)
		{
			structure = Scenes::Get()->GetStructure().get();
		}

		auto result = std::make_shared<GameObject>(transform);

		if (structure != nullptr)
		{
			structure->Add(result);
		}

		return result;
	}

	std::shared_ptr<GameObject> GameObject::Resource(const std::string &filepath, const Transform &transform, ISpatialStructure *structure)
	{
		if (structure == nullptr)
		{
			structure = Scenes::Get()->GetStructure().get();
		}

		auto result = std::make_shared<GameObject>(filepath, transform);

		if (structure != nullptr)
		{
			structure->Add(result);
		}

		return result;
	}

	GameObject::GameObject(const Transform &transform) :
		m_name(""),
		m_transform(transform),
		m_components(std::vector<std::shared_ptr<IComponent>>()),
		m_parent(),
		m_removed(false)
	{
	}

	GameObject::GameObject(const std::string &filepath, const Transform &transform) :
		m_name(""),
		m_transform(transform),
		m_components(std::vector<std::shared_ptr<IComponent>>()),
		m_parent(),
		m_removed(false)
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

	std::shared_ptr<IComponent> GameObject::AddComponent(const std::shared_ptr<IComponent> &component)
	{
		if (component == nullptr)
		{
			return nullptr;
		}

		component->SetGameObject(this);
		m_components.emplace_back(component);
		return component;
	}

	bool GameObject::RemoveComponent(const std::shared_ptr<IComponent> &component)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if (*it != nullptr && *it == component)
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
				auto componentName = Scenes::Get()->FindComponentName(*it);

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
