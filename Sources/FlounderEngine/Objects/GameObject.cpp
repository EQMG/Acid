#include "GameObject.hpp"

namespace Flounder
{

	GameObject::GameObject() :
		m_components(new std::vector<Component*>())
	{
	}

	GameObject::~GameObject()
	{
		for (auto component : *m_components)
		{
			delete component;
		}

		delete m_components;
	}

	void GameObject::AddComponent(Component *component)
	{
		m_components->push_back(component);
		component->SetGameObject(this);
	}

	void GameObject::RemoveComponent(Component *component)
	{
		for (auto it = m_components->begin(); it != m_components->end(); ++it)
		{
			if (*it == component)
			{
				component->SetGameObject(nullptr);
				delete component;
				m_components->erase(it);
				return;
			}
		}
	}
}
