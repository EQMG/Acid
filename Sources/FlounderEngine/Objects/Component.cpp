#include "Component.hpp"

#include "GameObject.hpp"

namespace Flounder
{
	Component::Component() :
		m_name(""),
		m_gameObject(nullptr),
		m_values(new std::map<unsigned int, ComponentGetSet>())
	{
	//	Link<std::string>(0, LINK_GET(std::string, GetName()), LINK_SET(std::string, SetName(v)));
	}

	Component::~Component()
	{
		delete m_values;
	}
	
	void Component::Update()
	{

	}

	void Component::Load(ComponentPrefab *componentPrefab)
	{
		for (unsigned int i = 0; i < componentPrefab->GetData().size(); i++)
		{
			auto value = m_values->find(i);

			if (value == m_values->end())
			{
				printf("Component index not found while loading: '%i'", i);
				continue;
			}

			(*value).second.setter(componentPrefab->GetString(i));
		}
	}

	void Component::Write(ComponentPrefab *componentPrefab)
	{
		for (const auto value : *m_values)
		{
			componentPrefab->SetRaw(value.first, value.second.getter());
		}
	}
}
