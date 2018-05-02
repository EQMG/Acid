#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Helpers/FormatString.hpp"
#include "Resources/Resources.hpp"
#include "Files/Json/FileJson.hpp"

namespace fl
{
	class GameObject;

	/// <summary>
	/// Class that represents a entity prefab.
	/// </summary>
	class FL_EXPORT PrefabObject :
		public IResource
	{
	private:
		std::string m_filename;
		FileJson *m_fileJson;
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

		LoadedValue *GetParent() const { return m_fileJson->GetParent(); }
	};
}