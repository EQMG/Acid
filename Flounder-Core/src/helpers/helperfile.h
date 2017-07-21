#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace flounder
{
	/// <summary>
	/// A helper for C++ files.
	/// </summary>
	class helperfile
	{
	public:
		/// <summary>
		/// Reads a text file into a string.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> The string containing the read file. </returns>
		static std::string readFile(const std::string &filepath)
		{
			std::ifstream ifs = std::ifstream(filepath.c_str());

			if (!ifs.is_open())
			{
				std::cout << "Could not find file: " << filepath << std::endl;
			}

			std::string content(
				std::istreambuf_iterator<char>(ifs.rdbuf()),
				std::istreambuf_iterator<char>()
			);
			return content;
		}
	};
}
