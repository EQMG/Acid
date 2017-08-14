#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
		static std::string readTextFile(const std::string &filepath)
		{
			std::ifstream ifs = std::ifstream(filepath.c_str());

			if (!ifs.is_open())
			{
				throw std::runtime_error("Could not find file: " + filepath);
			}

			std::string content(
				std::istreambuf_iterator<char>(ifs.rdbuf()),
				std::istreambuf_iterator<char>()
			);
			return content;
		}

		/// <summary>
		/// Reads a binary file into a char array.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> The char array loaded from the file. </returns>
		static std::vector<char> readBinaryFile(const std::string &filepath)
		{
			std::ifstream ifs = std::ifstream(filepath, std::ios::ate | std::ios::binary);

			if (!ifs.is_open())
			{
				throw std::runtime_error("Could not find file: "  + filepath);
			}

			size_t fileSize = (size_t)ifs.tellg();
			std::vector<char> buffer(fileSize);

			ifs.seekg(0);
			ifs.read(buffer.data(), fileSize);			ifs.close();

			return buffer;
		}
	};
}
