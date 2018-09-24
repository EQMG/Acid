#pragma once

#include <vector>
#include <iostream>
#include <optional>
#include "Engine/Engine.hpp"
#include "Zip/FileZip.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing files on engine updates.
	/// </summary>
	class ACID_EXPORT Files :
		public IModule
	{
	private:
		static std::vector<std::string> SEARCH_PATHS;
		static std::vector<std::unique_ptr<FileZip>> SEARCH_ZIPS;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Files *Get() { return Engine::Get()->GetModule<Files>(); }

		Files();

		void Update() override;

		static std::vector<std::string> GetSearchPaths() { return SEARCH_PATHS; }

		/// <summary>
		/// Adds an file search path.
		/// </summary>
		/// <param name="path"> The task to add. </param>
		static void AddSearchPath(const std::string &path);

		/// <summary>
		/// Find a real or partial path in a search path.
		/// </summary>
		/// <param name="path"> The path to find. </param>
		/// <returns> The path to the first found. </returns>
		static std::string Search(const std::string &path);

		/// <summary>
		/// Reads a file found by real or partial path.
		/// </summary>
		/// <param name="path"> The path to read. </param>
		/// <returns> The data read from the file. </returns>
		static std::optional<std::string> Read(const std::string &path);
	};
}
