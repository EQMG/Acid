#include "Component.hpp"

#include "GameObject.hpp"

namespace fl
{
	Component::Component() :
		m_name(""),
		m_gameObject(nullptr),
		m_enabled(true)
	{
	}

	Component::~Component()
	{
	}
}
