#include "EntityPrefab.hpp"

//#include "Components.hpp"

namespace Flounder
{
	EntityPrefab::EntityPrefab(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_fileCsv(new FileCsv(filename)),
		m_components(new std::unordered_map<std::string, ComponentPrefab*>())
	{
		m_fileCsv->Load();

		for (unsigned int i = 0; i < m_fileCsv->GetRowCount(); i++)
		{
			RowCsv row = m_fileCsv->GetRow(i);

			if (!row.m_elements.empty())
			{
				std::string name = row.m_elements.at(0);
				std::vector<std::string> data = std::vector<std::string>(row.m_elements);
				data.erase(data.begin());
				m_components->insert(std::make_pair(name, new ComponentPrefab(data)));
			}
		}
	}

	EntityPrefab::~EntityPrefab()
	{
		delete m_fileCsv;
		delete m_components;
	}

	void EntityPrefab::Save()
	{
		m_fileCsv->Clear();

		for (auto component : *m_components)
		{
		//	auto sample = Components::CreateComponent(component.first, nullptr);
		//	if (sample != nullptr)
		//	{
		//		sample->Save(component.second);
		//		delete sample;
		//	}

			std::vector<std::string> data = std::vector<std::string>();
			data.push_back(component.first);

			for (const auto &i : component.second->GetData())
			{
				data.push_back(i);
			}

			RowCsv row = RowCsv(data);
			m_fileCsv->PushRow(row);
		}

		m_fileCsv->Save();
	}

	ComponentPrefab *EntityPrefab::GetComponentData(const std::string &component)
	{
		auto it = m_components->find(component);

		if (it != m_components->end())
		{
			return (*it).second;
		}

		return nullptr;
	}

	std::vector<std::string> EntityPrefab::GetComponents()
	{
		std::vector<std::string> keys = std::vector<std::string>();

		for (auto it : *m_components) 
		{
			keys.push_back(it.first);
		}

		return keys;
	}
}
