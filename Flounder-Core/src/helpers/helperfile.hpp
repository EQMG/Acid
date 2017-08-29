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
			FILE* file = fopen(filepath.c_str(), "rt");

			if (file == nullptr)
			{
				throw std::runtime_error("Could not find file: " + filepath);
			}

			fseek(file, 0, SEEK_END);
			unsigned long length = ftell(file);
			char* data = new char[length + 1];
			memset(data, 0, length + 1);
			fseek(file, 0, SEEK_SET);
			fread(data, 1, length, file);
			fclose(file);

			std::string result = std::string(data);
			delete[] data;
			return result;
		}

		/// <summary>
		/// Reads a binary file into a char array.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> The char array loaded from the file. </returns>
		static std::vector<char> readBinaryFile(const std::string &filepath) // TODO: Move from ifsteam to normal C binary file loading.
		{
			std::ifstream ifs = std::ifstream(filepath, std::ios::ate | std::ios::binary);

			if (!ifs.is_open())
			{
				throw std::runtime_error("Could not find file: " + filepath);
			}

			size_t fileSize = (size_t) ifs.tellg();
			std::vector<char> buffer(fileSize);

			ifs.seekg(0);
			ifs.read(buffer.data(), fileSize);
			ifs.close();

			return buffer;
		}
	};
}
