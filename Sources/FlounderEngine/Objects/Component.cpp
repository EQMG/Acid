#include "Component.hpp"

#include "GameObject.hpp"

namespace Flounder
{
	Component::Component() :
		m_values(new std::map<unsigned int, ComponentGetSet*>()),
		m_name(""),
		m_gameObject(nullptr)
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
		/*printf("\"%s\": {\n", GetName().c_str());

		for (auto i : *m_values)
		{
			printf("  \"%s\": %s,\n", i.second->m_name.c_str(), (*i.second->m_getter)().c_str());
		}

		printf("},\n");*/

		for (unsigned int i = 0; i < componentPrefab->GetData().size(); i++)
		{
			auto value = m_values->find(i);

			if (value == m_values->end())
			{
				printf("Component link index '%i' not found while loading: '%s'\n", i, GetName().c_str());
				continue;
			}

			if ((*value).second->m_setter != nullptr)
			{
				(*(*value).second->m_setter)(componentPrefab, i);
			}
		}
	}

	void Component::Write(PrefabComponent *componentPrefab)
	{
		for (const auto &value : *m_values)
		{
			if (value.second->m_getter != nullptr)
			{
				componentPrefab->SetRaw(value.first, (*value.second->m_getter)());
			}
		}
	}
}
