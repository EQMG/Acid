#include "Entity.hpp"

#include "EntityPrefab.hpp"
#include "Files/FileSystem.hpp"
#include "Scenes.hpp"

namespace acid
{
Entity::Entity(const Transform& transform) : m_name(""), m_localTransform(transform), m_parent(nullptr), m_removed(false) {}

Entity::Entity(const std::string& filename, const Transform& transform) : Entity(transform)
{
	auto prefabObject = EntityPrefab::Create(filename);

	for(const auto& child : prefabObject->GetParent()->GetChildren())
		{
			if(child->GetName().empty())
				{
					continue;
				}

			auto component = Scenes::Get()->GetComponentRegister().Create(child->GetName());

			if(component == nullptr)
				{
					continue;
				}

			component->Decode(*child);
			AddComponent(component);
		}

	m_name = FileSystem::FileName(filename);
}

Entity::~Entity()
{
	if(m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}
}

void Entity::Update()
{
	for(auto it = m_components.begin(); it != m_components.end();)
		{
			if((*it)->IsRemoved())
				{
					it = m_components.erase(it);
					continue;
				}

			if((*it)->GetParent() != this)
				{
					(*it)->SetParent(this);
				}

			if((*it)->IsEnabled())
				{
					if(!(*it)->m_started)
						{
							(*it)->Start();
							(*it)->m_started = true;
						}

					(*it)->Update();
				}

			++it;
		}
}

Component* Entity::AddComponent(Component* component)
{
	if(component == nullptr)
		{
			return nullptr;
		}

	component->SetParent(this);
	m_components.emplace_back(component);
	return component;
}

void Entity::RemoveComponent(Component* component)
{
	m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [&](std::unique_ptr<Component>& c) { return c.get() == component; }), m_components.end());
}

void Entity::RemoveComponent(const std::string& name)
{
	m_components.erase(std::remove_if(m_components.begin(), m_components.end(),
									  [&](std::unique_ptr<Component>& c) {
										  auto componentName = Scenes::Get()->GetComponentRegister().FindName(c.get());
										  return componentName && name == *componentName;
									  }),
					   m_components.end());
}

Transform Entity::GetWorldTransform() const
{
	if(m_localTransform.IsDirty())
		{
			if(m_parent != nullptr)
				{
					m_worldTransform = m_parent->GetWorldTransform() * m_localTransform;
				}
			else
				{
					m_worldTransform = m_localTransform;
				}

			for(const auto& child : m_children)
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

void Entity::SetParent(Entity* parent)
{
	if(m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}

	m_parent = parent;

	if(m_parent != nullptr)
		{
			m_parent->AddChild(this);
		}
}

void Entity::AddChild(Entity* child)
{
	m_children.emplace_back(child);
}

void Entity::RemoveChild(Entity* child)
{
	m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}
}
