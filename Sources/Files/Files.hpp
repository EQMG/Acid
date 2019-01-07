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
		static void AddSearchPath(const std::string &path);

		/// <summary>
		/// Removes a file search path.
		/// </summary>
		/// <param name="path"> The path to remove. </param>
		static void RemoveSearchPath(const std::string &path);

		/// <summary>
		/// Reads a file found by real or partial path.
		/// </summary>
		/// <param name="path"> The path to read. </param>
		/// <returns> The data read from the file. </returns>
		static std::optional<std::string> Read(const std::string &path);
	};
}
