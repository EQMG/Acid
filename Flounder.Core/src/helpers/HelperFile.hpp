#pragma once

#include <vector>

namespace Flounder
{
	/// <summary>
	/// A helper for C++ files.
	/// </summary>
	class HelperFile
	{
	public:
		/// <summary>
		/// Reads a text file into a string.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> The string containing the read file. </returns>
		static std::string ReadTextFile(const std::string &filepath);

		/// <summary>
		/// Reads a binary file into a char array.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> The char array loaded from the file. </returns>
		static std::vector<char> ReadBinaryFile(const std::string &filepath);
	};
}
