#pragma once

#include <vector>
#include <cstring>
#include <string>

namespace Flounder
{
	/// <summary>
	/// A helper for C++ files.
	/// </summary>
	class HelperFile
	{
	public:
		/// <summary>
		/// Gets if a file exists.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> If the file exists. </returns>
		static bool FileExists(const std::string &filepath);

		/// <summary>
		/// Deletes a file.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		static void DeleteFile(const std::string &filepath);

		/// <summary>
		/// Deletes contents from a file, but leaves it.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		static void ClearFile(const std::string &filepath);

		/// <summary>
		/// Creates a file.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		static void CreateFile(const std::string &filepath);

		/// <summary>
		/// Creates a directory.
		/// </summary>
		/// <param name="path"> The directory to create. </param>
		static void CreateFolder(const std::string &path);

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

		/// <summary>
		/// Writes to a text file from a string.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="data"> The text data. </param>
		static void WriteTextFile(const std::string &filepath, const std::string &data);

		/// <summary>
		/// Writes to a binary file from a char vector.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="data"> The binary data. </param>
		static void WriteBinaryFile(const std::string &filepath, const std::vector<char> &data);

		static std::string GetWorkingDirectory();

		static std::string GetHomeDirectory();
	};
}
