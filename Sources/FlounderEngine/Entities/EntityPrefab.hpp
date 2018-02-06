#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "../Helpers/FormatString.hpp"
#include "../Resources/Resources.hpp"
#include "../Files/Csv/FileCsv.hpp"

namespace Flounder
{
	class ComponentPrefab
	{
	private:
		std::vector<std::string> m_data;
	public:
		ComponentPrefab(const std::vector<std::string> &data) :
			m_data(std::vector<std::string>(data))
		{
		}

		std::string GetRaw(const unsigned int &i)
		{
			return i >= m_data.size() ? "" : m_data.at(i);
		}

		std::string GetString(const unsigned int &i)
		{
			return FormatString::RemoveAll(GetRaw(i), '\"');
		}

		int GetInt(const unsigned int &i)
		{
			std::string data = GetRaw(i);
			return data.empty() ? 0 : atoi(data.c_str());
		}

		float GetFloat(const unsigned int &i)
		{
			std::string data = GetRaw(i);
			return data.empty() ? 0.0f : static_cast<float>(atof(data.c_str()));
		}

		bool GetBool(const unsigned int &i)
		{
			return GetInt(i) == 1;
		}

		std::vector<std::string> GetData() const { return m_data; }
	};

	/// <summary>
	/// Class that represents a entity prefab.
	/// </summary>
	class EntityPrefab :
		public IResource
	{
	private:
		std::string m_filename;
		FileCsv *m_fileCsv;
		std::unordered_map<std::string, ComponentPrefab*> *m_components;
	public:
		static EntityPrefab *Resource(const std::string &filename)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<EntityPrefab *>(resource);
			}

			EntityPrefab *result = new EntityPrefab(filename);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		/// <summary>
		/// Creates a new entity prefab.
		/// </summary>
		/// <param name="filename"> The file name. </param>
		EntityPrefab(const std::string &filename);

		~EntityPrefab();

		void Save();

		std::string GetFilename() override { return m_filename; }

		std::vector<std::string> GetComponents();

		ComponentPrefab *GetComponentData(const std::string &component);
	};
}