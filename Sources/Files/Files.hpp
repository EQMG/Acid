#pragma once

#include <vector>
#include "Engine/Engine.hpp"

namespace fl
{
	/// <summary>
	/// A module used for managing files on engine updates.
	/// </summary>
	class FL_EXPORT Files :
		public IModule
	{
	private:
		static std::vector<std::string> SEARCH_PATHS;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Files *Get()
		{
			return Engine::Get()->GetModule<Files>();
		}

		Files();

		~Files();

		void Update() override;

		static std::vector<std::string> GetSearchPaths() { return SEARCH_PATHS; }

		/// <summary>
		/// Adds an file search path.
		/// </summary>
		/// <param name="path"> The task to add. </param>
		static void AddSearchPath(const std::string &path);

		/// <summary>
		/// Find a file by partial path in a search path.
		/// </summary>
		/// <param name="filename"> The filename to find. </param>
		/// <returns> The path to the first file found. </returns>
		std::string SearchFile(const std::string &filename);
	};
}
