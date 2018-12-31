#include "Entity.hpp"

#include "Helpers/FileSystem.hpp"
#include "Scenes.hpp"
#include "EntityPrefab.hpp"

namespace acid
{
	Entity::Entity(const Transform &transform) :
		m_name(""),
		m_localTransform(transform),
		m_mutex(std::mutex()),
		m_components(std::vector<std::unique_ptr<Component>>()),
		m_parent(nullptr),
		m_children(std::vector<Entity *>()),
		m_removed(false)
	{
	}

	Entity::Entity(const std::string &filename, const Transform &transform) :
		Entity(transform)
	{
		auto prefabObject = EntityPrefab::Create(filename);

		for (auto &value : prefabObject->GetParent()->GetChildren())
		{
			if (value->GetName().empty())
			{
				continue;
			}

			auto component = Scenes::Get()->GetComponentRegister().Create(value->GetName());

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
		std::lock_guard<std::mutex> lock(m_mutex);

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
				if (!(*it)->m_started)
				{
					(*it)->Start();
					(*it)->m_started = true;
				}

				(*it)->Update();
			}

			++it;
		}
	}

	Component *Entity::AddComponent(Component *component)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (component == nullptr)
		{
			return nullptr;
		}

		component->SetParent(this);
		m_components.emplace_back(component);
		return component;
	}

	void Entity::RemoveComponent(Component *component)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it).get() == component)
			{
				(*it)->SetParent(nullptr);

				m_components.erase(it);
			}
		}
	}

	void Entity::RemoveComponent(const std::string &name)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			auto componentName = Scenes::Get()->GetComponentRegister().FindName((*it).get());

			if (componentName && name == *componentName)
			{
				continue;
			}

			(*it)->SetParent(nullptr);

			m_components.erase(it);
		}
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
