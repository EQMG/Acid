#include "Component.hpp"

#include "GameObject.hpp"

namespace Flounder
{
	Component::Component() :
		m_name(""),
		m_gameObject(nullptr),
		m_values(new std::map<unsigned int, ComponentGetSet*>())
	{
	}

	Component::~Component()
	{
		for (auto value : *m_values)
		{
			delete value.second;
		}

		delete m_values;
	}
	
	void Component::Update()
	{
	}

	void Component::Load(PrefabComponent *componentPrefab)
	{
		printf("Loading component: '%s'\n", GetName().c_str());

		for (unsigned int i = 0; i < componentPrefab->GetData().size(); i++)
		{
			auto value = m_values->find(i);

			if (value == m_values->end())
			{
				printf("Component link index '%i' not found while loading: '%s'\n", i, GetName().c_str());
				continue;
			}

			(*(*value).second->setter)(componentPrefab->GetString(i));
		}
	}

	void Component::Write(PrefabComponent *componentPrefab)
	{
		for (const auto &value : *m_values)
		{
			componentPrefab->SetRaw(value.first, (*value.second->getter)());
		}
	}
}
