#include "Component.hpp"

#include "GameObject.hpp"

namespace Flounder
{
	Component::Component() :
		m_name(""),
		m_gameObject(nullptr),
		m_values(new std::map<std::string, ComponentGetSet>())
	{
	//	Link<std::string>("Name", LINK_GET(std::string, GetName()), LINK_SET(std::string, SetName(v)));
	}

	Component::~Component()
	{
		delete m_values;
	}

	void Component::Load(ComponentPrefab *componentPrefab)
	{
		m_values->clear();

		for (const auto data : componentPrefab->GetData())
		{

		}
	}

	void Component::Write(ComponentPrefab *componentPrefab)
	{
		for (const auto value : *m_values)
		{

		}
	}
}
