#include "Component.hpp"

#include "GameObject.hpp"

namespace Flounder
{
	Component::Component() :
		m_name(""),
		m_gameObject(nullptr),
		m_values(new std::map<unsigned int, ComponentGetSet>())
	{
	}

	Component::~Component()
	{
		delete m_values;
	}
	
	void Component::Update()
	{
	}

	void Component::Load(PrefabComponent *componentPrefab)
	{
		printf("Loading: '%s'\n", GetName().c_str());

		for (unsigned int i = 0; i < componentPrefab->GetData().size(); i++)
		{
			auto value = m_values->find(i);

			if (value == m_values->end())
			{
				printf("Component '%s' index not found while loading: '%i'\n", GetName().c_str(), i);
				continue;
			}

		//	(*value).second.setter(componentPrefab->GetString(i));
		}
	}

	void Component::Write(PrefabComponent *componentPrefab)
	{
		for (const auto &value : *m_values)
		{
			componentPrefab->SetRaw(value.first, value.second.getter());
		}
	}
}
