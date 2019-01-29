#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Files/Files.hpp"
#include "Files/File.hpp"
#include "Helpers/String.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
	class Entity;

	/// <summary>
	/// Class that represents a entity prefab.
	/// </summary>
	class ACID_EXPORT EntityPrefab :
		public Resource
	{
	public:
		/// <summary>
		/// Will find an existing prefab object with the same filename, or create a new prefab object.
		/// </summary>
		/// <param name="filename"> The file to load the prefab object from. </param>
		static std::shared_ptr<EntityPrefab> Create(const std::string &filename);

		/// <summary>
		/// Creates a new entity prefab.
		/// </summary>
		/// <param name="filename"> The file name. </param>
		explicit EntityPrefab(const std::string &filename);

		void Encode(Metadata &metadata) const override;

		void Write(const Entity &entity);

		void Save();

		const std::string &GetFilename() const { return m_filename; }

		Metadata *GetParent() const { return m_file->GetMetadata(); }
	private:
		std::string m_filename;
		std::unique_ptr<File> m_file;
	};
}
