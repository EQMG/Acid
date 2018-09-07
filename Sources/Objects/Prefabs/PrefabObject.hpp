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
	class GameObject;

	/// <summary>
	/// Class that represents a entity prefab.
	/// </summary>
	class ACID_EXPORT PrefabObject :
		public IResource
	{
	private:
		std::string m_filename;
		std::unique_ptr<IFile> m_file;
		std::shared_ptr<Metadata> m_parent;
	public:
		/// <summary>
		/// Will find an existing prefab object with the same filename, or create a new prefab object.
		/// </summary>
		/// <param name="filename"> The file to load the prefab object from. </param>
		static std::shared_ptr<PrefabObject> Resource(const std::string &filename);

		/// <summary>
		/// Creates a new entity prefab.
		/// </summary>
		/// <param name="filename"> The file name. </param>
		PrefabObject(const std::string &filename);

		~PrefabObject();

		void Write(const GameObject &gameObject);

		void Save();

		std::string GetFilename() override { return m_filename; }

		std::shared_ptr<Metadata> GetParent() const { return m_parent; }
	};
}
