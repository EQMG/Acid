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
		std::vector<std::string> m_searchPaths;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Files *Get()
		{
			return reinterpret_cast<Files *>(Engine::Get()->GetModule("files"));
		}

		Files();

		~Files();

		void Update() override;

		std::vector<std::string> GetSearchPaths() const { return m_searchPaths; }

		/// <summary>
		/// Adds an file search path.
		/// </summary>
		/// <param name="path"> The task to add. </param>
		void AddSearchPath(const std::string &path);
	};
}
