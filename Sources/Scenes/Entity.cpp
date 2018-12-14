#include "Entity.hpp"

#include "Helpers/FileSystem.hpp"
#include "Scenes.hpp"
#include "EntityPrefab.hpp"

namespace acid
{
	Entity::Entity(const Transform &transform) :
		m_name(""),
		m_localTransform(transform),
		m_components(std::vector<std::unique_ptr<Component>>()),
		m_parent(nullptr),
		m_children(std::vector<Entity *>()),
		m_removed(false)
	{
	}

	Entity::Entity(const std::string &filename, const Transform &transform) :
		Entity(transform)
	{
		auto prefabObject = EntityPrefab::Resource(filename);

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

	Entity::~Entity()
	{
		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}
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

			if ((*it)->IsEnabled())
			{
				if (!(*it)->IsStarted())
				{
					(*it)->Start();
					(*it)->SetStarted(true);
				}

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
		if (m_localTransform.IsDirty())
		{
			if (m_parent != nullptr)
			{
				m_worldTransform = m_parent->GetWorldTransform() * m_localTransform;
			}
			else
			{
				m_worldTransform = m_localTransform;
			}

			for (auto &child : m_children)
			{
				child->m_localTransform.SetDirty(true);
			}

			m_localTransform.SetDirty(false);
		}

		return m_worldTransform;
	}

	Matrix4 Entity::GetWorldMatrix() const
	{
		return GetWorldTransform().GetWorldMatrix();
	}

	void Entity::SetParent(Entity *parent)
	{
		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}

		m_parent = parent;

		if (m_parent != nullptr)
		{
			m_parent->AddChild(this);
		}
	}

	void Entity::AddChild(Entity *child)
	{
		m_children.emplace_back(child);
	}

	void Entity::RemoveChild(Entity *child)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
	}
}
