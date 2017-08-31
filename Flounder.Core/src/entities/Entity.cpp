#include "Entity.hpp"

namespace Flounder
{
	Entity::Entity(ISpatialStructure<Entity*> *structure, const Vector3 &position, const Vector3 &rotation) :
		m_structure(structure),
		m_components(new std::vector<IComponent*>()),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_removed(false)
	{
		if (m_structure != nullptr)
		{
			m_structure->Add(this);
		}
	}

	Entity::~Entity()
	{
		Remove();

		for (auto c : *m_components)
		{
			delete c;
		}

		delete m_components;

		delete m_position;
		delete m_rotation;
	}

	void Entity::Update()
	{
		for (auto c : *m_components)
		{
			c->Update();
		}
	}

	void Entity::MoveStructure(ISpatialStructure<Entity*> *structure)
	{
		m_structure->Remove(this);
		structure->Add(this);
		m_structure = structure;
	}

	void Entity::AddComponent(IComponent *component)
	{
		m_components->push_back(component);
		component->SetEntity(this);
	}

	void Entity::RemoveComponent(IComponent *component)
	{
		for (auto it = m_components->begin(); it != m_components->end(); ++it)
		{
			if (*it == component)
			{
				component->SetEntity(nullptr);
				delete component;
				m_components->erase(it);
				return;
			}
		}
	}

	template<class t>
	t Entity::GetComponent()
	{
		for (auto c : *m_components)
		{
			t casted = dynamic_cast<t>(c);

			if (casted != nullptr)
			{
				return casted;
			}
		}

		return nullptr;
	}

	icollider *Entity::GetCollider()
	{
		return nullptr;
	}

	void Entity::Remove()
	{
		m_removed = true;
		m_structure->Remove(this);
	}
}
