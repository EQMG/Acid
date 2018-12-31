#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Files/Files.hpp"
#include "Files/IFile.hpp"
#include "Helpers/String.hpp"
#include "Resources/IResource.hpp"

namespace acid
{
	class Entity;

	/// <summary>
	/// Class that represents a entity prefab.
	/// </summary>
	class ACID_EXPORT EntityPrefab :
		public IResource
	{
	private:
		std::string m_filename;
		std::unique_ptr<IFile> m_file;
		Metadata *m_parent;
	public:
		/// <summary>
		/// Will find an existing prefab object with the same filename, or create a new prefab object.
		/// </summary>
		/// <param name="filename"> The file to load the prefab object from. </param>
		static std::shared_ptr<EntityPrefab> Resource(const std::string &filename);

		/// <summary>
		/// Creates a new entity prefab.
		/// </summary>
		/// <param name="filename"> The file name. </param>
		explicit EntityPrefab(const std::string &filename);

		void Write(const Entity &entity);

		void Save();

		std::string GetFilename() const { return m_filename; }

		Metadata *GetParent() const { return m_parent; }
	};
}
