#pragma once

#include <vector>
#include <iostream>
#include <optional>
#include "Engine/Engine.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing files on engine updates.
	/// </summary>
	class ACID_EXPORT Files :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Files *Get() { return Engine::Get()->GetModuleManager().Get<Files>(); }

		Files();

		~Files();

		void Update() override;

		/// <summary>
		/// Adds an file search path, ensure <seealso cref="#SetBaseDirectory()"/> is called once before.
		/// </summary>
		/// <param name="path"> The path to add. </param>
		void AddSearchPath(const std::string &path);

		/// <summary>
		/// Removes a file search path.
		/// </summary>
		/// <param name="path"> The path to remove. </param>
		void RemoveSearchPath(const std::string &path);

		/// <summary>
		/// Clears all file search paths.
		/// </summary>
		void ClearSearchPath();

		/// <summary>
		/// Reads a file found by real or partial path.
		/// </summary>
		/// <param name="path"> The path to read. </param>
		/// <returns> The data read from the file. </returns>
		static std::optional<std::string> Read(const std::string &path);

		/// <summary>
		/// Finds all the files in a path.
		/// </summary>
		/// <param name="path"> The path to search. </param>
		/// <param name="recursive"> If paths will be recursively searched. </param>
		/// <returns> The files found. </returns>
		static std::vector<std::string> FilesInPath(const std::string &path, const bool &recursive = true);

	private:
		std::vector<std::string> m_searchPaths;
	};
}
