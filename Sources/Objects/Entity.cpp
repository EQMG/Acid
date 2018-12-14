#include "Entity.hpp"

#include "Helpers/FileSystem.hpp"
#include "Prefabs/PrefabObject.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	Entity::Entity(const Transform &transform) :
		m_name(""),
		m_localTransform(transform),
		m_components(std::vector<std::unique_ptr<Component>>()),
		m_parent(nullptr),
		m_removed(false)
	{
	}

	Entity::Entity(const std::string &filename, const Transform &transform) :
		Entity(transform)
	{
		auto prefabObject = PrefabObject::Resource(filename);

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

		m_name = FileSystem::FileName(filename);
	}

	void Entity::Update()
	{
		for (auto it = m_components.begin(); it != m_components.end();)
		{
			if ((*it)->IsRemoved())
			{
				it = m_components.erase(it);
				continue;
			}

			if ((*it)->GetParent() != this)
			{
				(*it)->SetParent(this);
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

			++it;
		}
	}

	Component *Entity::AddComponent(Component *component)
	{
		if (component == nullptr)
		{
			return nullptr;
		}

		component->SetParent(this);
		m_components.emplace_back(component);
		return component;
	}

	bool Entity::RemoveComponent(Component *component)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it).get() == component)
			{
				(*it)->SetParent(nullptr);

				m_components.erase(it);
				return true;
			}
		}

		return false;
	}

	bool Entity::RemoveComponent(const std::string &name)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			auto componentName = Scenes::Get()->FindComponentName((*it).get());

			if (componentName && name == *componentName)
			{
				continue;
			}

			(*it)->SetParent(nullptr);

			m_components.erase(it);
			return true;
		}

		return false;
	}

	Transform Entity::GetWorldTransform() const
	{
		if (m_parent == nullptr)
		{
		//	if (m_localTransform.IsDirty())
		//	{
				m_worldTransform = m_localTransform;
		//		m_localTransform.SetDirty(false);
		//	}
		}
		else
		{
		//	if (m_localTransform.IsDirty() || m_parent->m_localTransform.IsDirty())
		//	{
				m_worldTransform = m_parent->GetWorldTransform() * m_localTransform;
		//		m_localTransform.SetDirty(false);
		//	}
		}

		return m_worldTransform;
	}

	Matrix4 Entity::GetWorldMatrix() const
	{
		return GetWorldTransform().GetWorldMatrix();
	}
}
