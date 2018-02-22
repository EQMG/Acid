#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../../Helpers/FormatString.hpp"
#include "../../Resources/Resources.hpp"
#include "../../Files/Csv/FileCsv.hpp"
#include "PrefabComponent.hpp"

namespace Flounder
{
	class GameObject;

	/// <summary>
	/// Class that represents a entity prefab.
	/// </summary>
	class F_EXPORT PrefabObject :
		public IResource
	{
	private:
		std::string m_filename;
		FileCsv *m_fileCsv;
		std::unordered_map<std::string, PrefabComponent *> *m_components;
	public:
		static PrefabObject *Resource(const std::string &filename)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<PrefabObject *>(resource);
			}

			PrefabObject *result = new PrefabObject(filename);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		/// <summary>
		/// Creates a new entity prefab.
		/// </summary>
		/// <param name="filename"> The file name. </param>
		PrefabObject(const std::string &filename);

		~PrefabObject();

		void Write(GameObject *gameObject);

		void Save();

		std::string GetFilename() override { return m_filename; }

		std::vector<std::string> GetComponents();

		PrefabComponent *GetComponentData(const std::string &component);
	};
}