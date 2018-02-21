#include "Component.hpp"

#include "GameObject.hpp"

namespace Flounder
{
	Component::Component() :
		m_name(""),
		m_gameObject(nullptr)
	{
	}

	Component::~Component()
	{
	//	m_gameObject->RemoveComponent(this);
	}
}
