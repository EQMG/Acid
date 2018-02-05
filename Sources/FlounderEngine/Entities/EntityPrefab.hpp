#pragma once

#include <string>
#include <vector>
#include <map>
#include "../Resources/Resources.hpp"
#include "../Files/Csv/FileCsv.hpp"

namespace Flounder
{
	class ComponentPrefab
	{
	public:
		std::vector<std::string> m_data;

		ComponentPrefab(const std::vector<std::string> &data) :
			m_data(std::vector<std::string>(data))
		{
		}
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
		std::map<std::string, ComponentPrefab*> *m_components;
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